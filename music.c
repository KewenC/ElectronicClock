#include"music.h"
unsigned char timer0h, timer0l, timer;
uint isShow = 1;//1为显示，0为隐藏
uchar Native_Flag = 0;
uchar mHour,mMinute,mHour2,mMinute2;
uint dd = 0;
//--------------------------------------
// 频率-半周期数据表 高八位     本软件共保存了四个八度的28个频率数据
code unsigned char FREQH[] = {
    0xF2, 0xF3, 0xF5, 0xF5, 0xF6, 0xF7, 0xF8,    //低音1234567
    0xF9, 0xF9, 0xFA, 0xFA, 0xFB, 0xFB, 0xFC, 0xFC,//1,2,3,4,5,6,7,i
    0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE,            //高音 234567
    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF};   //超高音 1234567

// 频率-半周期数据表 低八位
code unsigned char FREQL[] = {
    0x42, 0xC1, 0x17, 0xB6, 0xD0, 0xD1, 0xB6,    //低音1234567
    0x21, 0xE1, 0x8C, 0xD8, 0x68, 0xE9, 0x5B, 0x8F, //1,2,3,4,5,6,7,i
    0xEE, 0x44, 0x6B, 0xB4, 0xF4, 0x2D,             //高音 234567
    0x47, 0x77, 0xA2, 0xB6, 0xDA, 0xFA, 0x16};   //超高音 1234567
//--------------------------------------
//一个音符有三个数字。前为第几个音、中为第几个八度、后为时长（以半拍为单位）。

code unsigned char sszymmh[] = {//送别
5,2,2,3,2,1,5,2,1,1,3,4,6,2,2,1,3,2,5,2,4,5,2,2,1,2,1,2,2,1,3,1,1,2,2,1,1,2,1,2,2,4,
5,2,2,3,2,1,5,2,1,1,3,3,7,2,1,6,2,2,1,3,2,5,2,4,5,2,2,2,2,1,3,2,1,4,2,3,7,1,1,1,2,4,};

code unsigned char sszymmh2[] = {//望
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
code unsigned char sszymmh1[] = {//天空之城
6,1,1,7,1,1,1,2,3,7,1,1,1,2,2,3,2,2,7,1,5,3,1,2,6,1,3,5,1,1,6,1,2,1,2,2,
5,1,5,3,1,2,4,1,3,3,1,1,4,1,1,1,2,1,1,2,2,3,1,4,1,2,2};

/*
 * 写Lcd字符串函数
 */
void WriteDataString(unsigned char *str){
    while(*str){
        LcdWriteData(*str);
        str++;
    }
}

/*
 * 中断函数
 */
void t0int() interrupt 1         //T0中断程序，控制发音的音调
{
    TR0 = 0;                     //先关闭T0
    speaker = !speaker;          //输出方波, 发音
    TH0 = timer0h;               //下次的中断时间, 这个时间, 控制音调高低
    TL0 = timer0l;
    TR0 = 1;                     //启动T0
}
//--------------------------------------
void delay1(unsigned char t)     //延时程序，控制发音的时间长度
{
    unsigned char t1;
    unsigned long t2;
	if(isShow){
		   if(Native_Flag){
				 LcdWriteCom(0x80+0X40);
    			 LcdWriteData('0'+mHour2/16);				//时
  				 LcdWriteData('0'+(mHour2&0x0f));
   				 LcdWriteData(':');
  				 LcdWriteData('0'+mMinute2/16);				//分
  				 LcdWriteData('0'+(mMinute2&0x0f));
				 WriteDataString("   ");
				 LcdWriteCom(0x80+0x4e);
				 WriteDataString("A2");
		   }else{
				 LcdWriteCom(0x80+0X40);
    			 LcdWriteData('0'+mHour/16);				//时
  				 LcdWriteData('0'+(mHour&0x0f));
   				 LcdWriteData(':');
  				 LcdWriteData('0'+mMinute/16);				//分
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
    for(t1 = 0; t1 < t; t1++)    //双重循环, 共延时t个半拍
      for(t2 = 0; t2 < 4000; t2++); //延时期间, 可进入T0中断去发音	  8000
    TR0 = 0;                        //关闭T0, 停止发音
}
//--------------------------------------
void song()                      //演奏一个音符
{
    TH0 = timer0h;               //控制音调
    TL0 = timer0l;
    TR0 = 1;                     //启动T0, 由T0输出方波去发音	
    delay1(timer);                 //控制时间长度
}
//--------------------------------------
/*
 * 闹铃音乐
 */
uchar AlarmMusic(uchar a,uchar h,uchar m,uchar s)
{
    unsigned char k, i, j,num;
    TMOD = 1;                    //置T0定时工作方式1
    ET0 = 1;                     //开T0中断
    EA = 1;                      //开CPU中断
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
			   k = sszymmh[i] + 7 * sszymmh[i + 1] - 1;//第i个是音符, 第i+1个是第几个八度
        	   timer = sszymmh[i + 2];   //读出时间长度数值
			   j= sizeof(sszymmh)/sizeof(char);
		   break;
		   case 1:
			   k = sszymmh1[i] + 7 * sszymmh1[i + 1] - 1;//第i个是音符, 第i+1个是第几个八度
        	   timer = sszymmh1[i + 2];   //读出时间长度数值
			   j= sizeof(sszymmh1)/sizeof(char);
		   break;
		   case 2:
			   k = sszymmh2[i] + 7 * sszymmh2[i + 1] - 1;//第i个是音符, 第i+1个是第几个八度
        	   timer = sszymmh2[i + 2];   //读出时间长度数值
			   j= sizeof(sszymmh2)/sizeof(char);
		   break;
		}
		timer0h = FREQH[k];      //从数据表中读出频率数值
        timer0l = FREQL[k];      //实际上, 是定时的时间长度
        i += 3;
		Feast=(!Feast);
        song();                  //发出一个音符
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

