/*******************************************************************************
*                 
*                              ����ʱ��Դ����
*
*******************************************************************************/
//����ͷ�ļ�
#include<reg51.h>
#include"lcd.h"
#include"ds1302.h"
#include"music.h"
//��������
uint Mode_Flag = 0;
uint Set_Flag = 0;//0-hour 1-minute 2-year 3-month 4-day
uint Set_Flag_Alarm	= 0;//0-alarm hour    1-alarm minute
uint d = 0;
uint is_flicker = 1;//1Ϊ��ʾ��0Ϊ����
uchar isAlarm = 0; //��һ�������־λ
uchar isAlarm2 = 0; //�ڶ��������־λ
uchar Alarm_Hour = 6;//��һ������
uchar Alarm_Minute = 0;
uchar Alarm_Hour2 = 0x12;//�ڶ�������
uchar Alarm_Minute2 = 0;
uchar Sleep_Minute = 0;//˯�� ��
uchar isSleep = 0; //�Ƿ�˯��
uchar WhichSleep = 0;//��1������2˯��
uchar f;
uchar Alarm_Sound = 0;//����ѡ��
uchar code Feast_month[12] = {0x01,0x02,0x03,0x03,0x05,0x05,0x06,0x08,0x09,0x09,0x10,0x12};	//����
uchar code Feast_day[12] =   {0x01,0x14,0x08,0x12,0x01,0x04,0x01,0x01,0x10,0x18,0x01,0x25};
//��������
void SleepAlarm(uchar Minute,uchar Which);
void SetAlarmSound();
void EnterSetAlarmSound();
void DeepOne();
void DeepNumber(uchar x);
void AlarmUpAndDown();
void delay(unsigned int i);
void UpAndDown(uchar ind,uchar H,uchar L);
void SetAlarmTime();
void SetAllTime();
void EnterSetAlarm();
void EnterSetTime();
void LcdDisplay();
void LcdDisplay_time();
void EnterMode();

/*
 * ��һ������
 */
void DeepOne(){
    ALARM=0;
    Lcd1602_Delay1ms(200);
    ALARM=1;
    Lcd1602_Delay1ms(500);
}

/*
 * �켸������
 */
void DeepNumber(uchar x)
{
    uchar i;
    uchar j;
    x = x==0?24:x;
    j = x<6?100:0;
    for(i=0;i<x;i++){
        ALARM=0;
        Lcd1602_Delay1ms(100);
        ALARM=1;
        Ds1302ReadTime();
        LcdDisplay();
        Lcd1602_Delay1ms(j);
    }
}
/*
 * ��ʱ����
 */
void delay(unsigned int i)
{
    char j;
    for(i; i > 0; i--)
        for(j = 200; j > 0; j--);
}

/*
 *Lcdд�ַ�������
 */
void LcdWriteDataString(unsigned char *str){
    while(*str){
        LcdWriteData(*str);
        str++;
    }
}

/*
 * ������������
 */
void AlarmUpAndDown(uchar flag,uchar H,uchar a){
    if(UP==0){
        Lcd1602_Delay1ms(10);
        if(UP==0){
            uint tmp = flag==0?Alarm_Hour:Alarm_Minute;
            tmp = (tmp&0x0f)<0x9?(tmp+0x01):(tmp+0x07);
            if(!flag){
                Alarm_Hour = tmp<(H+1)?tmp:0 ;
            }else{
                Alarm_Minute = tmp<(H+1)?tmp:0 ;
            }
            if(a){
                isAlarm2=1;
            }else{
                isAlarm=1;
            }
            DeepOne();
            while((d<50)&&(UP==0)){
                Lcd1602_Delay1ms(10);
                d++;
            }
            d=0;
        }
    }
    if(DOWN==0){
        Lcd1602_Delay1ms(10);
        if(DOWN==0){

            uint tmp = flag==0?Alarm_Hour:Alarm_Minute;
            if(tmp){
                tmp = (tmp&0x0f)>0?(tmp-0x01):(tmp-0x07);
                if(!flag){
                    Alarm_Hour = tmp ;
                }else{
                    Alarm_Minute = tmp ;
                }
            }else{
                if(!flag){
                    Alarm_Hour = H ;
                }else{
                    Alarm_Minute = H ;
                }
            }
            if(a){
                isAlarm2=1;
            }else{
                isAlarm=1;
            }
            DeepOne();
            while((d<50)&&(DOWN==0)){
                Lcd1602_Delay1ms(10);
                d++;
            }
            d=0;
        }
    }
}

/*
 * ʱ����������
 */
void UpAndDown(uchar ind,uchar H,uchar L){
    if(UP==0){
        Lcd1602_Delay1ms(10);
        if(UP==0){
            uint tmp = Ds1302SingleRead(ind);
            tmp = (tmp&0x0f)<0x9?(tmp+0x01):(tmp+0x07);
            Ds1302SingleWrite(ind,tmp<(H+1)?tmp:L) ;
            DeepOne();
            while((d<50)&&(UP==0)){
                Lcd1602_Delay1ms(10);
                d++;
            }
            d=0;
        }
    }
    if(DOWN==0){
        Lcd1602_Delay1ms(10);
        if(DOWN==0){

            uint tmp = Ds1302SingleRead(ind);
            if(tmp){
                tmp = (tmp&0x0f)>0?(tmp-0x01):(tmp-0x07);
                if(L&&tmp==0){
                    Ds1302SingleWrite(ind,H) ;
                }else{
                    Ds1302SingleWrite(ind,tmp) ;
                }
            }else{
                Ds1302SingleWrite(ind,H) ;
            }
            DeepOne();
            while((d<50)&&(DOWN==0)){
                Lcd1602_Delay1ms(10);
                d++;
            }
            d=0;
        }
    }
}

/*
 *��������ʱ��
 */
void SetAlarmTime(uchar a){
    uchar tmp_alarm_hour;
    uchar tmp_alarm_minute;
    if(a){
        tmp_alarm_hour = Alarm_Hour;
        tmp_alarm_minute = Alarm_Minute;
        Alarm_Hour = Alarm_Hour2;
        Alarm_Minute = Alarm_Minute2;
    }
    switch(Set_Flag_Alarm){
        case 0://��������ʱ
            LcdWriteCom(0x80);
            if(a){
                LcdWriteDataString("SetAlarm2:hour");
            }else{
                LcdWriteDataString("SetAlarm1:hour");
            }
            LcdWriteCom(0x80+0X40);
            if(is_flicker){
                LcdWriteData('0'+Alarm_Hour/16);				//ʱ
                LcdWriteData('0'+(Alarm_Hour&0x0f));
                is_flicker=0;
            }else{
                LcdWriteDataString("  ");
                is_flicker=1;
            }
            LcdWriteData(':');
            LcdWriteData('0'+Alarm_Minute/16);				//��
            LcdWriteData('0'+(Alarm_Minute&0x0f));
            AlarmUpAndDown(0,0x23,a);
            break;
        case 1:	 //���������
            LcdWriteCom(0x80);
            if(a){
                LcdWriteDataString("SetAlarm2:minute");
            }else{
                LcdWriteDataString("SetAlarm1:minute");
            }

            LcdWriteCom(0x80+0X40);
            LcdWriteData('0'+Alarm_Hour/16);				//ʱ
            LcdWriteData('0'+(Alarm_Hour&0x0f));
            LcdWriteData(':');
            if(is_flicker){
                LcdWriteData('0'+Alarm_Minute/16);				//��
                LcdWriteData('0'+(Alarm_Minute&0x0f));
                is_flicker=0;
            }else{
                LcdWriteDataString("  ");
                is_flicker=1;
            }
            AlarmUpAndDown(1,0x59,a);
            break;

    }
    if(a){
        Alarm_Hour2 = Alarm_Hour;
        Alarm_Minute2 = Alarm_Minute;
        Alarm_Hour = tmp_alarm_hour ;
        Alarm_Minute = tmp_alarm_minute;
    }
}

/*
 *���ö�Ӧ��ʱ��
 */
void SetAllTime(){
    LcdWriteCom(0x80);
    Ds1302ReadTime();
    switch(Set_Flag){
        case 0:
            LcdWriteDataString("SetTime:hour");
            LcdWriteCom(0x80+0X40);
            if(is_flicker){
                LcdWriteData('0'+TIME[2]/16);				//ʱ
                LcdWriteData('0'+(TIME[2]&0x0f));
                is_flicker=0;
            }else{
                LcdWriteDataString("  ");
                is_flicker=1;
            }
            LcdWriteData(':');
            LcdWriteData('0'+TIME[1]/16);				//��
            LcdWriteData('0'+(TIME[1]&0x0f));
            LcdWriteData(':');
            LcdWriteData('0'+TIME[0]/16);				//��
            LcdWriteData('0'+(TIME[0]&0x0f));
            UpAndDown(2,0x23,0);
            break;
        case 1:
            LcdWriteDataString("SetTime:Minute");
            LcdWriteCom(0x80+0X40);
            LcdWriteData('0'+TIME[2]/16);				//ʱ
            LcdWriteData('0'+(TIME[2]&0x0f));
            LcdWriteData(':');
            if(is_flicker){
                LcdWriteData('0'+TIME[1]/16);				//��
                LcdWriteData('0'+(TIME[1]&0x0f));
                is_flicker=0;
            }else{
                LcdWriteDataString("  ");
                is_flicker=1;
            }
            LcdWriteData(':');
            LcdWriteData('0'+TIME[0]/16);				//��
            LcdWriteData('0'+(TIME[0]&0x0f));
            UpAndDown(1,0x59,0);
            break;
        case 2:
            LcdWriteDataString("SetTime:Second");
            LcdWriteCom(0x80+0X40);
            LcdWriteData('0'+TIME[2]/16);				//ʱ
            LcdWriteData('0'+(TIME[2]&0x0f));
            LcdWriteData(':');
            LcdWriteData('0'+TIME[1]/16);				//��
            LcdWriteData('0'+(TIME[1]&0x0f));
            LcdWriteData(':');
            if(is_flicker){
                LcdWriteData('0'+TIME[0]/16);				//��
                LcdWriteData('0'+(TIME[0]&0x0f));
                is_flicker=0;
            }else{
                LcdWriteDataString("  ");
                is_flicker=1;
            }
            UpAndDown(0,0x59,0);
            break;
        case 3:
            LcdWriteDataString("SetTime:year");
            LcdWriteCom(0x80+0X40);
            if(is_flicker){
                LcdWriteData('2');
                LcdWriteData('0');
                LcdWriteData('0'+TIME[6]/16);			//��
                LcdWriteData('0'+(TIME[6]&0x0f));
                is_flicker=0;
            }else{
                LcdWriteDataString("    ");
                is_flicker=1;
            }
            LcdWriteData('-');
            LcdWriteData('0'+TIME[4]/16);			//��
            LcdWriteData('0'+(TIME[4]&0x0f));
            LcdWriteData('-');
            LcdWriteData('0'+TIME[3]/16);			//��
            LcdWriteData('0'+(TIME[3]&0x0f));
            LcdWriteCom(0x80+0x4b);
            LcdWriteData('0'+(TIME[5]&0x07));	//����
            UpAndDown(6,0x99,0);
            break;
        case 4:
            LcdWriteDataString("SetTime:month");
            LcdWriteCom(0x80+0X40);
            LcdWriteData('2');
            LcdWriteData('0');
            LcdWriteData('0'+TIME[6]/16);			//��
            LcdWriteData('0'+(TIME[6]&0x0f));
            LcdWriteData('-');
            if(is_flicker){
                LcdWriteData('0'+TIME[4]/16);			//��
                LcdWriteData('0'+(TIME[4]&0x0f));
                is_flicker=0;
            }else{
                LcdWriteDataString("  ");
                is_flicker=1;
            }
            LcdWriteData('-');
            LcdWriteData('0'+TIME[3]/16);			//��
            LcdWriteData('0'+(TIME[3]&0x0f));
            UpAndDown(4,0x12,1);
            LcdWriteCom(0x80+0x4b);
            LcdWriteData('0'+(TIME[5]&0x07));	//����
            break;
        case 5:
            LcdWriteDataString("SetTime:day");
            LcdWriteCom(0x80+0X40);
            LcdWriteData('2');
            LcdWriteData('0');
            LcdWriteData('0'+TIME[6]/16);			//��
            LcdWriteData('0'+(TIME[6]&0x0f));
            LcdWriteData('-');
            LcdWriteData('0'+TIME[4]/16);			//��
            LcdWriteData('0'+(TIME[4]&0x0f));
            LcdWriteData('-');
            if(is_flicker){
                LcdWriteData('0'+TIME[3]/16);			//��
                LcdWriteData('0'+(TIME[3]&0x0f));
                is_flicker=0;
            }else{
                LcdWriteDataString("  ");
                is_flicker=1;
            }
            LcdWriteCom(0x80+0x4b);
            LcdWriteData('0'+(TIME[5]&0x07));	//����
            UpAndDown(3,0x31,1);
            break;
        case 6:
            LcdWriteDataString("SetTime:week");
            LcdWriteCom(0x80+0X40);
            LcdWriteData('2');
            LcdWriteData('0');
            LcdWriteData('0'+TIME[6]/16);			//��
            LcdWriteData('0'+(TIME[6]&0x0f));
            LcdWriteData('-');
            LcdWriteData('0'+TIME[4]/16);			//��
            LcdWriteData('0'+(TIME[4]&0x0f));
            LcdWriteData('-');
            LcdWriteData('0'+TIME[3]/16);			//��
            LcdWriteData('0'+(TIME[3]&0x0f));
            if(is_flicker){
                LcdWriteCom(0x80+0x4b);
                LcdWriteData('0'+(TIME[5]&0x07));	//����
                is_flicker=0;
            }else{
                LcdWriteDataString("  ");
                is_flicker=1;
            }
            UpAndDown(5,0x7,1);
            break;
    }
}

/********************************************************
*                      ������
*********************************************************/
void main()
{
    Ds1302Init();
    LcdInit();
    KeyInit();
    ALARM=1;
    Feast=1;
    while(1)
    {
        if(MODE==0){
            Lcd1602_Delay1ms(10);
            if(MODE==0){
                DeepOne();
                EnterMode();
            }
        }else{	 //����״̬
            Ds1302ReadTime();
            LcdDisplay();
            if(isSleep){//˯��
                if(Ds1302SingleRead(1)==Sleep_Minute){
                    if(AlarmMusic(WhichSleep,Ds1302SingleRead(2),Sleep_Minute,Alarm_Sound)){
                        SleepAlarm(Sleep_Minute,WhichSleep);
                    }else{
                        isSleep = 0;
                    }
                }
            }
            if(isAlarm){//����1
                if(Ds1302SingleRead(2)==Alarm_Hour&&Ds1302SingleRead(1)==Alarm_Minute)	{
                    //LcdWriteCom(0x01);  //����
                    Ds1302ReadTime();
                    LcdDisplay();
                    if(AlarmMusic(0,Alarm_Hour,Alarm_Minute,Alarm_Sound)){//����1����˯��
                        SleepAlarm(Alarm_Minute,0);
                        isAlarm=0;
                    }else{
                        isAlarm= 0;
                        isSleep = 0;
                    }

                }
            }
            if(isAlarm2){//����2
                if(Ds1302SingleRead(2)==Alarm_Hour2&&Ds1302SingleRead(1)==Alarm_Minute2)	{
                    //LcdWriteCom(0x01);  //����
                    //DeepNumber(2);
                    Ds1302ReadTime();
                    LcdDisplay();
                    if(AlarmMusic(1,Alarm_Hour2,Alarm_Minute2,Alarm_Sound)){
                        SleepAlarm(Alarm_Minute2,1);
                        isAlarm2=0;
                    }else{
                        isAlarm2 = 0;
                        isSleep = 0;
                    }
                }
            }
            if(!Ds1302SingleRead(0)&&!Ds1302SingleRead(1)&&(Ds1302SingleRead(2)/16*10+(Ds1302SingleRead(2)&0x0f)>=6)&&(Ds1302SingleRead(2)/16*10+(Ds1302SingleRead(2)&0x0f)<=22)){
                //ÿ��60������������������켸���������㱨ʱ��
                DeepNumber(Ds1302SingleRead(2)/16*10+(Ds1302SingleRead(2)&0x0f));
            }else if(!Ds1302SingleRead(0)){
                DeepOne();	//ÿ��60���Զ�����һ��	   TODO **************************************************************************************************************************��***************
                Feast=0;
            }
            //���չ���
            for(f=0;f<12;f++){
                if(Ds1302SingleRead(4)==Feast_month[f]&& Ds1302SingleRead(3)==Feast_day[f]){
                    Feast=0;
                    break;
                }else{
                    Feast=1;
                }
            }

            if(UP==0){
                Lcd1602_Delay1ms(10);
                if(UP==0){
                    DeepOne();
                    isAlarm = isAlarm?0:1 ;
                    while((d<50)&&(UP==0)){
                        Lcd1602_Delay1ms(10);
                        d++;
                    }
                    d=0;
                }
            }
            if(DOWN==0){
                Lcd1602_Delay1ms(10);
                if(DOWN==0){
                    DeepOne();
                    isAlarm2 = isAlarm2?0:1;
                    while((d<50)&&(DOWN==0)){
                        Lcd1602_Delay1ms(10);
                        d++;
                    }
                    d=0;
                }
            }

        }

    }

}

/*
 *չʾsound list
 */
void SetAlarmSound()
{
    LcdWriteCom(0x80);
    LcdWriteDataString("SetMusic:");
    LcdWriteCom(0x80+0X40);
    switch(Alarm_Sound){
        case 0:
            LcdWriteDataString("songbie");
            break;
        case 1:
            LcdWriteDataString("tiankongzhicheng");
            break;
        case 2:
            LcdWriteDataString("wang");
            break;
    }
}

/*
 *����������������
 */
void EnterSetAlarmSound()
{
    SetAlarmSound();
    if(SET==0){
        Lcd1602_Delay1ms(10);
        if(SET==0){
            DeepOne();
            LcdWriteCom(0x01);  //����
            ++Alarm_Sound;
            Alarm_Sound%=3;
            while((d<50)&&(SET==0)){
                Lcd1602_Delay1ms(10);
                d++;
            }
            d=0;
        }
    }
}

/*
 *������������
 */
void EnterSetAlarm(uchar a)
{
    SetAlarmTime(a);
    if(SET==0){
        Lcd1602_Delay1ms(10);
        if(SET==0){
            DeepOne();
            LcdWriteCom(0x01);  //����
            ++Set_Flag_Alarm;
            Set_Flag_Alarm%=2;
            while((d<50)&&(SET==0)){
                Lcd1602_Delay1ms(10);
                d++;
            }
            d=0;
        }
    }
}

/*
 *��������ʱ��
 */
void EnterSetTime()
{
    SetAllTime();

    if(SET==0){
        Lcd1602_Delay1ms(10);
        if(SET==0){
            DeepOne();
            LcdWriteCom(0x01);  //����
            ++Set_Flag;
            Set_Flag%=7;
            while((d<50)&&(SET==0)){
                Lcd1602_Delay1ms(10);
                d++;
            }
            d=0;
        }
    }

}

/*
 * ����Modeģʽ
 */
void EnterMode()
{
    LcdWriteCom(0x01);  //����
    //Lcd1602_Delay1ms(300);
    while(1){
        if(Mode_Flag==0){  //enter set time
            EnterSetTime();
        }else if(Mode_Flag==1){	   //enter set Alarm
            EnterSetAlarm(0);
        }else if(Mode_Flag==2){	   //enter set ALarm2
            EnterSetAlarm(1);
        }else if(Mode_Flag==3){
            EnterSetAlarmSound();  //enter set Alarm Sound
        }else{		 //back home
            Mode_Flag=0;
            break;
        }
        if(MODE==0){
            Lcd1602_Delay1ms(10);
            if(MODE==0){
                DeepOne();
                LcdWriteCom(0x01);  //����
                ++Mode_Flag;
                Mode_Flag%=5;
                while((d<50)&&(MODE==0)){
                    Lcd1602_Delay1ms(10);
                    d++;
                }
                d=0;
            }
        }
    }
}

/*
 * ��ʾ����
 */
void LcdDisplay()
{
    LcdWriteCom(0x80+0X40);
    LcdWriteData('0'+TIME[2]/16);				//ʱ
    LcdWriteData('0'+(TIME[2]&0x0f));
    LcdWriteData(':');
    LcdWriteData('0'+TIME[1]/16);				//��
    LcdWriteData('0'+(TIME[1]&0x0f));
    LcdWriteData(':');
    LcdWriteData('0'+TIME[0]/16);				//��
    LcdWriteData('0'+(TIME[0]&0x0f));
    if(isAlarm){
        LcdWriteCom(0x80+0x4b);
        LcdWriteDataString("A1");
    }else{
        LcdWriteCom(0x80+0x4b);
        LcdWriteDataString("  ");
    }
    if(isAlarm2){
        LcdWriteCom(0x80+0x4e);
        LcdWriteDataString("A2");
    }else{
        LcdWriteCom(0x80+0x4e);
        LcdWriteDataString("  ");
    }

    LcdWriteCom(0x80);
    LcdWriteData('2');
    LcdWriteData('0');
    LcdWriteData('0'+TIME[6]/16);			//��
    LcdWriteData('0'+(TIME[6]&0x0f));
    LcdWriteData('-');
    LcdWriteData('0'+TIME[4]/16);			//��
    LcdWriteData('0'+(TIME[4]&0x0f));
    LcdWriteData('-');
    LcdWriteData('0'+TIME[3]/16);			//��
    LcdWriteData('0'+(TIME[3]&0x0f));
    LcdWriteCom(0x8D);
    LcdWriteData('0'+(TIME[5]&0x07));	//����	


}
/*
void LcdDisplay_time()
{
	if(isAlarm){
		LcdWriteCom(0x80+0x4b);
		LcdWriteDataString("A1");
	}else{
		LcdWriteCom(0x80+0x4b);
		LcdWriteDataString("  ");
	}	
	if(isAlarm2){
	   	LcdWriteCom(0x80+0x4e);
		LcdWriteDataString("A2");
	}else{
		LcdWriteCom(0x80+0x4e);
		LcdWriteDataString("  ");
	}
}
*/

/*
 * ����˯��
 */
void SleepAlarm(uchar Minute,uchar Which)
{
    uchar i;
    isSleep = 1;
    WhichSleep = Which;
    Sleep_Minute = Minute ;
    for(i=0;i<2;i++){
        Sleep_Minute = (Sleep_Minute&0x0f)<0x9?(Sleep_Minute+0x01):(Sleep_Minute+0x07);
        Sleep_Minute = Sleep_Minute<0x60?Sleep_Minute:0 ;
    }
}