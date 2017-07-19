/********************************************************************
* 文件名  ： 温度采集DS18B20.c
* 描述    :  该文件实现了用温度传感器件DS18B20对温度的采集，并在数码管上显示出来。
* 创建人  ： 东流，2009年4月10日
* 版本号  ： 2.0
***********************************************************************/
#include"ds18b20.h"
#include"lcd1602.h"
#include"uart.h"
#include"dht11.h"
#include "module_config.h"

/********************************************************************
* 名称 : Delay_1ms()
* 功能 : 延时子程序，延时时间为 1ms * x
* 输入 : x (延时一毫秒的个数)
* 输出 : 无
***********************************************************************/
void Delay_1ms(uint ms)//1ms延时
{
#if 0
	uchar x,j;
	for(j=0;j<i;j++)
	for(x=0;x<=148;x++);	
#else
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
#endif
}
#if 0
unsigned char Float2Char(float value, char *array)
{
	int IntegerPart;
	float DecimalPart;
	uchar i = 0;
	uchar j = 0;
	char temp;
	if(value >= 1)
	{
		 IntegerPart = (char)value;
		 DecimalPart = value - IntegerPart;
	}
	else
	{
		 IntegerPart = (int)value;
		 DecimalPart = value - (float)IntegerPart;
	}

	if(IntegerPart == 0)
	{
		 array[0] = '0';
		 array[1] = '.';
		 i = 1;
	}
	else
	{
			while(IntegerPart > 0)
			{
			array[i] = (char)(IntegerPart % 10) + '0';
			IntegerPart /= 10;
			i++;
			}
			i--;

			//fix the result
			for(j=0; j<i; j++)
			{
				 temp = array[j];
				 array[j] = array[i - j];
				 array[i - j] = temp;
			}
			i++;
			array[i] = '.';
	}
	//convert the Decimalpart
	i++;
	array[i++] = (char)((uint)(DecimalPart * 10)%10) + '0';
	array[i++] = (char)((uint)(DecimalPart * 100)%10) + '0';
	array[i++] = (char)((uint)(DecimalPart * 1000)%10) + '0';
	array[i++] = (char)((uint)(DecimalPart * 10000)%10) + '0';
	array[i]   = '\0';
	return i;
}
#endif
/********************************************************************
* 名称 : Main()
* 功能 : 主函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void main()
{
	float temp = 0;
	BYTE i;
	char dis[10]={0};
	char RH_buf[5] = {0};
	char T_buf[5] = {0};

	uart_init();
	lcd_init();// 初始化LCD
	
	while(1)
	{
		ds18b20_temp_get(dis);
		
		RH_T_get(RH_buf, T_buf);
		
		uart_send(dis);
		uart_send(RH_buf);
		uart_send(T_buf);

		lcd_wcmd(0x06);//向右移动光标
		lcd_pos(0);//设置显示位置为第一行的第1个字符
		i = 0;
		while(dis[i] != '\0')
		{   //显示字符
			lcd_wdat(dis[ i ]);
			i++;
		}

		i = 0;
		while(RH_buf[i] != '\0')
		{   //显示字符
			lcd_wdat(RH_buf[ i ]);
			i++;
		}
		i = 0;
		while(T_buf[i] != '\0')
		{   //显示字符
			lcd_wdat(T_buf[ i ]);
			i++;
		}
		
		Delay_1ms(1000);//控制停留时间

		lcd_wcmd(0x01);//清除LCD的显示内容
	}
}
