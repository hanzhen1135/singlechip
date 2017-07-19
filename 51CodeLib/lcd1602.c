#include"lcd1602.h"
#ifdef LCD1602_MODULE

#if 0
/********************************************************************
* 文件名  ： 液晶1602显示.c
* 描述    :  该程序实现了对液晶1602的控制。
* 创建人  ： 东流，2009年4月10日
* 版本号  ： 2.0
***********************************************************************/

/********************************************************************
* 名称 : delay()
* 功能 : 延时,延时时间大概为140US。
* 输入 : 无
* 输出 : 无
***********************************************************************/

void delay()
{
	int i,j;
	for(i=0; i<=100; i++)
	for(j=0; j<=20; j++)
;
}

/********************************************************************
* 名称 : enable(uchar del)
* 功能 : 1602命令函数
* 输入 : 输入的命令值
* 输出 : 无
***********************************************************************/

void enable(uchar del)
{
	P0 = del;
	RS = 0;
	RW = 0;
	E = 0;
	delay();
	E = 1;
	delay();
}

/********************************************************************
* 名称 : write(uchar del)
* 功能 : 1602写数据函数
* 输入 : 需要写入1602的数据
* 输出 : 无
***********************************************************************/

void write(uchar del)
{
	P0 = del;
	RS = 1;
	RW = 0;
	E = 0;
	delay();
	E = 1;
	delay();
}
	
/********************************************************************
* 名称 : L1602_init()
* 功能 : 1602初始化，请参考1602的资料
* 输入 : 无
* 输出 : 无
***********************************************************************/
void L1602_init(void)
{
	enable(0x01);
	enable(0x38);
	enable(0x0c);
	enable(0x06);
	enable(0xd0);
}

/********************************************************************
* 名称 : L1602_char(uchar line,uchar row,char sign)
* 功能 : 改变液晶中某位的值，如果要让第一行，第五个字符显示"b" ，调用该函数如下
		 L1602_char(1,5,'b')
* 输入 : 行，列，需要输入1602的数据
* 输出 : 无
***********************************************************************/
void L1602_char(uchar line,uchar row,char sign)
{
	uchar a;
	if(line == 1) a = 0x80;
	if(line == 2) a = 0xc0;
	a = a + row - 1;
	enable(a);
	write(sign);
}

/********************************************************************
* 名称 : L1602_string(uchar line,uchar row,uchar *p)
* 功能 : 改变液晶中某位的值，如果要让第一行，第五个字符开始显示"ab cd ef" ，调用该函数如下
	 	 L1602_string(1,5,"ab cd ef;")
* 输入 : 行，列，需要输入1602的数据
* 输出 : 无
***********************************************************************/
void L1602_string(uchar line,uchar row,uchar *p)
{
	uchar a;
	if(line == 1) a = 0x80;
	if(line == 2) a = 0xc0;
	a = a + row - 1;
	enable(a);
	while(1)
	{
		if(*p == '\0') break;
		write(*p);
		p++;
	}
}

#if 0
/********************************************************************
* 名称 : 
* 功能 : 
* 输入 : 无
* 输出 : 无
***********************************************************************/
void main()
{
	L1602_init();
	L1602_char(1,5,'b');
//	L1602_string(1,1,"   0123456789 ");
	L1602_string(2,1,"   ABCDEDGHIJ ");
	while(1);	
}
#endif
#else

/****************延时函数************************/	
	void delay(int ms)
	{                    //延时子程序
	int i;
	while(ms--)
	{
	  for(i = 0; i< 250; i++)
	  {
	   _nop_();	  //空执行
	   _nop_();
	   _nop_();
	   _nop_();
	  }
	}
	}

/****************侧忙函数************************/		
	BOOL lcd_bz()
	{                         
	BOOL result;
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	result = (BOOL)(P0 & 0x80);	 //检测P0最高位是否为1
	LCD_EP = 0;
	return result;//返回侧忙结果
	}

/****************写命令函数************************/
		
	void lcd_wcmd_8bit(int cmd)
	{                          // 写入指令数据到LCD
	while(lcd_bz());
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EP = 0;
	_nop_();
	_nop_(); 
	P0 = cmd; //将高4位位指令通过P0口传给1602
	_nop_();
	_nop_(); //用于产生一个脉冲宽度
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0;  
	}	
		
	void lcd_wcmd(int cmd)
	{                          // 写入指令数据到LCD
	while(lcd_bz());
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EP = 0;
	_nop_();
	_nop_(); 
	P0 = cmd; //将高4位位指令通过P0口传给1602
	_nop_();
	_nop_(); //用于产生一个脉冲宽度
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0;  
	P0 = (cmd&0x0f)<<4; //将低4位指令通过P0口传给1602
	_nop_();
	_nop_(); //用于产生一个脉冲宽度
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0;  
	}
	
	void lcd_pos(BYTE pos)
	{                          //设定显示位置
	lcd_wcmd(pos | 0x80);
	}

/****************写数据函数************************/	
	void lcd_wdat(BYTE dat) 
	{                   //写入字符显示数据到LCD
	while(lcd_bz());
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EP = 0;
	P0 = dat ;  //将高4位数据通过P0口传给1602
	_nop_();
	_nop_();   //用于产生一个脉冲宽度
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0; 

	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EP = 0;
		_nop_();
	_nop_();
	_nop_();
	_nop_();
	P0 = (dat&0x0f)<<4;  //将低4位位数据通过P0口传给1602
	_nop_();
	_nop_();   //用于产生一个脉冲宽度
	_nop_();
	_nop_();
	LCD_EP = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EP = 0; 
	}
	
void lcd_init()					//LCD初始化设定
{ 
		lcd_wcmd_8bit(0x38);                       //16*2显示，5*7点阵，8位数据
	delay(1);  
	lcd_wcmd_8bit(0x38);                       //16*2显示，5*7点阵，8位数据
	delay(1);  
	lcd_wcmd_8bit(0x28);     //16*2显示，5*7点阵，8位数据
	delay(1);  
	lcd_wcmd(0x28);
	delay(1);
		lcd_wcmd(0x0c);     //显示开，关光标
	delay(1);
		lcd_wcmd(0x02);     //显示开，关光标
	delay(1);
		lcd_wcmd(0x01);     //清除LCD的显示内容
	delay(1);

	/*	lcd_wcmd(0x38);     //16*2显示，5*7点阵，8位数据
	delay(1);
	lcd_wcmd(0x08);     //显示开，关光标
	delay(1);
	//	lcd_wcmd(0x04);     //移动光标
	delay(1);
	lcd_wcmd(0x02);     // 4位选择
	delay(1);
	lcd_wcmd(0x01);     //清除LCD的显示内容
	delay(1); */
}
#endif

#else /*LCD1602_MODULE*/

void lcd_wcmd(int cmd){}
void lcd_pos(BYTE pos){}
void lcd_wdat(BYTE dat){}
void lcd_init(){}

#endif /*LCD1602_MODULE*/