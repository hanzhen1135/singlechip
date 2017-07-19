/********************************************************************
* �ļ���  �� �¶Ȳɼ�DS18B20.c
* ����    :  ���ļ�ʵ�������¶ȴ�������DS18B20���¶ȵĲɼ����������������ʾ������
* ������  �� ������2009��4��10��
* �汾��  �� 2.0
***********************************************************************/
#include"ds18b20.h"
#include"lcd1602.h"
#include"uart.h"
#include"dht11.h"
#include "module_config.h"

/********************************************************************
* ���� : Delay_1ms()
* ���� : ��ʱ�ӳ�����ʱʱ��Ϊ 1ms * x
* ���� : x (��ʱһ����ĸ���)
* ��� : ��
***********************************************************************/
void Delay_1ms(uint ms)//1ms��ʱ
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
	   _nop_();	  //��ִ��
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
* ���� : Main()
* ���� : ������
* ���� : ��
* ��� : ��
***********************************************************************/
void main()
{
	float temp = 0;
	BYTE i;
	char dis[10]={0};
	char RH_buf[5] = {0};
	char T_buf[5] = {0};

	uart_init();
	lcd_init();// ��ʼ��LCD
	
	while(1)
	{
		ds18b20_temp_get(dis);
		
		RH_T_get(RH_buf, T_buf);
		
		uart_send(dis);
		uart_send(RH_buf);
		uart_send(T_buf);

		lcd_wcmd(0x06);//�����ƶ����
		lcd_pos(0);//������ʾλ��Ϊ��һ�еĵ�1���ַ�
		i = 0;
		while(dis[i] != '\0')
		{   //��ʾ�ַ�
			lcd_wdat(dis[ i ]);
			i++;
		}

		i = 0;
		while(RH_buf[i] != '\0')
		{   //��ʾ�ַ�
			lcd_wdat(RH_buf[ i ]);
			i++;
		}
		i = 0;
		while(T_buf[i] != '\0')
		{   //��ʾ�ַ�
			lcd_wdat(T_buf[ i ]);
			i++;
		}
		
		Delay_1ms(1000);//����ͣ��ʱ��

		lcd_wcmd(0x01);//���LCD����ʾ����
	}
}
