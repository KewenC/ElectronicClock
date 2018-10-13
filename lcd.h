#ifndef __LCD_H_
#define __LCD_H_
/**********************************
��ʹ�õ���4λ���ݴ����ʱ���壬
ʹ��8λȡ���������
**********************************/
#define LCD1602_4PINS

/**********************************
����ͷ�ļ�
**********************************/
#include<reg51.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/**********************************
PIN�ڶ���
**********************************/
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^7;  //7releast    7����
sbit LCD1602_RW=P2^6; //6           5   
sbit LCD1602_RS=P2^5; //5           6
sbit MODE=P2^0;
sbit SET=P2^1;
sbit UP=P2^2;
sbit DOWN=P2^3;
sbit ALARM=P2^4;
sbit Feast=P1^1;
/**********************************
��������
**********************************/
/*��51��Ƭ��12MHZʱ���µ���ʱ����*/
void Lcd1602_Delay1ms(uint c);   //��� 0us
/*LCD1602д��8λ�����Ӻ���*/
void LcdWriteCom(uchar com);
/*LCD1602д��8λ�����Ӻ���*/	
void LcdWriteData(uchar dat)	;
/*LCD1602��ʼ���ӳ���*/		
void LcdInit();					  
/*init Key*/
void KeyInit();
#endif
