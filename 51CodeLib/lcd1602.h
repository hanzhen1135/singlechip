#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "module_config.h"

#if 0
/*Dong liu*/
//这三个引脚参考资料
sbit E=P2^7;		//1602使能引脚
sbit RW=P2^6;		//1602读写引脚	
sbit RS=P2^5;		//1602数据/命令选择引脚

extern void L1602_init(void);
extern void L1602_char(uchar line,uchar row,char sign);
extern void L1602_string(uchar line,uchar row,uchar *p);

#else

sbit LCD_RS = P2^5;  //复位端            
sbit LCD_RW = P2^6;	 //写数据端
sbit LCD_EP = P2^7;	 //使能端
extern	void lcd_wcmd(int cmd);
extern	void lcd_pos(BYTE pos);
extern	void lcd_wdat(BYTE dat);
extern	void lcd_init();
#endif

#endif