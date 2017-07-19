#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "module_config.h"

#if 0
/*Dong liu*/
//���������Ųο�����
sbit E=P2^7;		//1602ʹ������
sbit RW=P2^6;		//1602��д����	
sbit RS=P2^5;		//1602����/����ѡ������

extern void L1602_init(void);
extern void L1602_char(uchar line,uchar row,char sign);
extern void L1602_string(uchar line,uchar row,uchar *p);

#else

sbit LCD_RS = P2^5;  //��λ��            
sbit LCD_RW = P2^6;	 //д���ݶ�
sbit LCD_EP = P2^7;	 //ʹ�ܶ�
extern	void lcd_wcmd(int cmd);
extern	void lcd_pos(BYTE pos);
extern	void lcd_wdat(BYTE dat);
extern	void lcd_init();
#endif

#endif