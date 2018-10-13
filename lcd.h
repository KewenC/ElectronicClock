#ifndef __LCD_H_
#define __LCD_H_
/**********************************
当使用的是4位数据传输的时候定义，
使用8位取消这个定义
**********************************/
#define LCD1602_4PINS

/**********************************
包含头文件
**********************************/
#include<reg51.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/**********************************
PIN口定义
**********************************/
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^7;  //7releast    7调试
sbit LCD1602_RW=P2^6; //6           5   
sbit LCD1602_RS=P2^5; //5           6
sbit MODE=P2^0;
sbit SET=P2^1;
sbit UP=P2^2;
sbit DOWN=P2^3;
sbit ALARM=P2^4;
sbit Feast=P1^1;
/**********************************
函数声明
**********************************/
/*在51单片机12MHZ时钟下的延时函数*/
void Lcd1602_Delay1ms(uint c);   //误差 0us
/*LCD1602写入8位命令子函数*/
void LcdWriteCom(uchar com);
/*LCD1602写入8位数据子函数*/	
void LcdWriteData(uchar dat)	;
/*LCD1602初始化子程序*/		
void LcdInit();					  
/*init Key*/
void KeyInit();
#endif
