#include<reg51.h>
#include"lcd.h"
sbit speaker = P2^4;
void t0int();
void delay1(unsigned char t);
void song();
uchar AlarmMusic(uchar,uchar,uchar,uchar);