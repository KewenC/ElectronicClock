#include"music.h"
unsigned char timer0h, timer0l, timer;
uint isShow = 1;//1Ϊ��ʾ��0Ϊ����
uchar Native_Flag = 0;
uchar mHour,mMinute,mHour2,mMinute2;
uint dd = 0;
//--------------------------------------
// Ƶ��-���������ݱ� �߰�λ     ��������������ĸ��˶ȵ�28��Ƶ������
code unsigned char FREQH[] = {
    0xF2, 0xF3, 0xF5, 0xF5, 0xF6, 0xF7, 0xF8,    //����1234567
    0xF9, 0xF9, 0xFA, 0xFA, 0xFB, 0xFB, 0xFC, 0xFC,//1,2,3,4,5,6,7,i
    0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE,            //���� 234567
    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF};   //������ 1234567

// Ƶ��-���������ݱ� �Ͱ�λ
code unsigned char FREQL[] = {
    0x42, 0xC1, 0x17, 0xB6, 0xD0, 0xD1, 0xB6,    //����1234567
    0x21, 0xE1, 0x8C, 0xD8, 0x68, 0xE9, 0x5B, 0x8F, //1,2,3,4,5,6,7,i
    0xEE, 0x44, 0x6B, 0xB4, 0xF4, 0x2D,             //���� 234567
    0x47, 0x77, 0xA2, 0xB6, 0xDA, 0xFA, 0x16};   //������ 1234567
//--------------------------------------
//һ���������������֡�ǰΪ�ڼ���������Ϊ�ڼ����˶ȡ���Ϊʱ�����԰���Ϊ��λ����

code unsigned char sszymmh[] = {//�ͱ�
5,2,2,3,2,1,5,2,1,1,3,4,6,2,2,1,3,2,5,2,4,5,2,2,1,2,1,2,2,1,3,1,1,2,2,1,1,2,1,2,2,4,
5,2,2,3,2,1,5,2,1,1,3,3,7,2,1,6,2,2,1,3,2,5,2,4,5,2,2,2,2,1,3,2,1,4,2,3,7,1,1,1,2,4,};

code unsigned char sszymmh2[] = {//��
1,3,2,7,2,2,6,2,2,5,2,2,3,2,2,2,2,1,3,2,1,3,2,1,5,2,2,
5,2,2,5,2,2,5,2,2,2,2,1,3,2,1,3,2,4,
1,3,2,7,2,2,6,2,1,5,2,1,5,2,2,3,2,2,2,2,1,3,2,1,5,2,2,
5,2,2,5,2,2,6,2,2,7,2,2,1,3,5,1,3,7};
/*
,//...
1,3,1,1,3,1,1,3,1,1,3,1,1,3,1,1,3,1,1,3,1,1,3,1,7,2,2,6,2,1,5,2,1,5,2,1,3,2,1,5,2,1,3,2,1,
5,2,2,3,2,2,5,2,2,5,2,2,3,2,2,5,2,2,3,2,2,6,2,5,
1,3,1,1,3,1,1,3,1,1,3,1,1,3,1,1,3,1,1,3,1,1,3,1,2,3,2,7,2,2,6,2,2,6,2,2,5,2,2,5,2,2,3,2,2,
5,2,2,3,2,2,5,2,2,5,2,2,3,2,2,5,2,2,6,2,2,6,2,3,1,3,2,7,2,2,6,2,2,5,2,2,
4,2,3,1,3,1,7,2,1,6,2,1,5,2,1,6,2,2,5,2,1,6,2,1,6,2,1,5,2,1,6,2,1,5,2,1,
6,2,2,5,2,2,6,2,2,7,2,1,1,3,1,1,3,3,6,2,1,1,3,1,6,2,1,3,3,1,
3,3,1,6,2,1,6,2,2,6,2,1,1,3,1,6,2,1,3,3,1,3,3,1,6,2,1,6,2,2,6,2,1,1,3,1,6,2,1,3,3,1,3,3,7};
*/
code unsigned char sszymmh1[] = {//���֮��
6,1,1,7,1,1,1,2,3,7,1,1,1,2,2,3,2,2,7,1,5,3,1,2,6,1,3,5,1,1,6,1,2,1,2,2,
5,1,5,3,1,2,4,1,3,3,1,1,4,1,1,1,2,1,1,2,2,3,1,4,1,2,2};

/*
 * дLcd�ַ�������
 */
void WriteDataString(unsigned char *str){
    while(*str){
        LcdWriteData(*str);
        str++;
    }
}

/*
 * �жϺ���
 */
void t0int() interrupt 1         //T0�жϳ��򣬿��Ʒ���������
{
    TR0 = 0;                     //�ȹر�T0
    speaker = !speaker;          //�������, ����
    TH0 = timer0h;               //�´ε��ж�ʱ��, ���ʱ��, ���������ߵ�
    TL0 = timer0l;
    TR0 = 1;                     //����T0
}
//--------------------------------------
void delay1(unsigned char t)     //��ʱ���򣬿��Ʒ�����ʱ�䳤��
{
    unsigned char t1;
    unsigned long t2;
	if(isShow){
		   if(Native_Flag){
				 LcdWriteCom(0x80+0X40);
    			 LcdWriteData('0'+mHour2/16);				//ʱ
  				 LcdWriteData('0'+(mHour2&0x0f));
   				 LcdWriteData(':');
  				 LcdWriteData('0'+mMinute2/16);				//��
  				 LcdWriteData('0'+(mMinute2&0x0f));
				 WriteDataString("   ");
				 LcdWriteCom(0x80+0x4e);
				 WriteDataString("A2");
		   }else{
				 LcdWriteCom(0x80+0X40);
    			 LcdWriteData('0'+mHour/16);				//ʱ
  				 LcdWriteData('0'+(mHour&0x0f));
   				 LcdWriteData(':');
  				 LcdWriteData('0'+mMinute/16);				//��
  				 LcdWriteData('0'+(mMinute&0x0f));
				 WriteDataString("   ");
				 LcdWriteCom(0x80+0x4b);
				 WriteDataString("A1");
		   }
		   isShow=0;
	}else{
		   LcdWriteCom(0x80+0X40);
		   WriteDataString("                ");
		   isShow=1;
	}
    for(t1 = 0; t1 < t; t1++)    //˫��ѭ��, ����ʱt������
      for(t2 = 0; t2 < 4000; t2++); //��ʱ�ڼ�, �ɽ���T0�ж�ȥ����	  8000
    TR0 = 0;                        //�ر�T0, ֹͣ����
}
//--------------------------------------
void song()                      //����һ������
{
    TH0 = timer0h;               //��������
    TL0 = timer0l;
    TR0 = 1;                     //����T0, ��T0�������ȥ����	
    delay1(timer);                 //����ʱ�䳤��
}
//--------------------------------------
/*
 * ��������
 */
uchar AlarmMusic(uchar a,uchar h,uchar m,uchar s)
{
    unsigned char k, i, j,num;
    TMOD = 1;                    //��T0��ʱ������ʽ1
    ET0 = 1;                     //��T0�ж�
    EA = 1;                      //��CPU�ж�
	Native_Flag = a;
	if(Native_Flag){
		mHour2=h;
		mMinute2=m;
	}else{
		mHour=h;
		mMinute=m;
	}
	Feast=0;
	num=0;
    while(1) {
	  num++;
      i = 0;
      timer = 1; 
      while(timer) {
	  	switch(s){
		   case 0:
			   k = sszymmh[i] + 7 * sszymmh[i + 1] - 1;//��i��������, ��i+1���ǵڼ����˶�
        	   timer = sszymmh[i + 2];   //����ʱ�䳤����ֵ
			   j= sizeof(sszymmh)/sizeof(char);
		   break;
		   case 1:
			   k = sszymmh1[i] + 7 * sszymmh1[i + 1] - 1;//��i��������, ��i+1���ǵڼ����˶�
        	   timer = sszymmh1[i + 2];   //����ʱ�䳤����ֵ
			   j= sizeof(sszymmh1)/sizeof(char);
		   break;
		   case 2:
			   k = sszymmh2[i] + 7 * sszymmh2[i + 1] - 1;//��i��������, ��i+1���ǵڼ����˶�
        	   timer = sszymmh2[i + 2];   //����ʱ�䳤����ֵ
			   j= sizeof(sszymmh2)/sizeof(char);
		   break;
		}
		timer0h = FREQH[k];      //�����ݱ��ж���Ƶ����ֵ
        timer0l = FREQL[k];      //ʵ����, �Ƕ�ʱ��ʱ�䳤��
        i += 3;
		Feast=(!Feast);
        song();                  //����һ������
		if(i>=j)	//sizeof(array) / sizeof(array[0])
		 {
		timer=0;
		}
		if(SET==0){
				Lcd1602_Delay1ms(10);
				if(SET==0){
					speaker = 1;
					TR0 = 0;
					return 0;
				while((dd<50)&&(SET==0)){
                    Lcd1602_Delay1ms(10);
                    dd++;
                }
                dd=0;
            	}
			}

		}
		if(num==3){
		   speaker = 1;
		   TR0 = 0;
		   return 1;
		}  
	} 
}

