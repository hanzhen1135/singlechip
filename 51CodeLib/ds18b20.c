#include "ds18b20.h"

#ifdef DS18B20_MODULE

#if 0 /*dongliu*/

#define jump_ROM 0xCC
#define start 0x44
#define read_EEROM 0xBE

/********************************************************************
* 名称 : delay()
* 功能 : 延时函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void delay(uint N)
{
	int i;
	for(i=0; i<N; i++)
	;
}

/********************************************************************
* 名称 : Reset()
* 功能 : 复位DS18B20
* 输入 : 无
* 输出 : 无
***********************************************************************/
uchar Reset(void)
{
	uchar deceive_ready;
	DQ = 0;
	delay(29);
	DQ = 1;
	delay(3);
	deceive_ready = DQ;
	delay(25);
	return(deceive_ready);
}

/********************************************************************
* 名称 : read_bit()
* 功能 : 从DS18B20读一个位值
* 输入 : 无
* 输出 : 从DS18B20读出的一个位值
***********************************************************************/
uchar read_bit(void)
{
	uchar i;
	DQ = 0;
	DQ = 1;
	for(i=0; i<3; i++);
	return(DQ);
}

/********************************************************************
* 名称 : write_bit()
* 功能 : 向DS18B20写一位
* 输入 : bitval（要对DS18B20写入的位值）
* 输出 : 无
***********************************************************************/
void write_bit(uchar bitval)
{
	DQ=0;if(bitval==1)
	DQ=1;
	delay(5);
	DQ=1;
}

/********************************************************************
* 名称 : read_byte()
* 功能 : 从DS18B20读一个字节
* 输入 : 无
* 输出 : 从DS18B20读到的值
***********************************************************************/
uchar read_byte(void)
{
	uchar i,m,receive_data;
	m = 1;
	receive_data = 0;
	for(i=0; i<8; i++)
	{
		if(read_bit())
		{
			receive_data = receive_data + (m << i);
		}
		delay(6);
	}
	return(receive_data);
}

/********************************************************************
* 名称 : write_byte()
* 功能 : 向DS18B20写一个字节
* 输入 : val（要对DS18B20写入的命令值）
* 输出 : 无
***********************************************************************/
void write_byte(uchar val)
{
	uchar i,temp;
	for(i=0; i<8; i++)
	{
		temp = val >> i;
		temp = temp & 0x01;
		write_bit(temp);
		delay(5);
	}
}

/*****************************************************************
* 名称 : ds18b20_temp_get(char *buf)
* 功能 : 获取ds18b20温度
* 输入 : val（要对DS18B20写入的命令值）
* 输出 : buf 温度字符串
******************************************************************/
void ds18b20_temp_get(char *buf)
{
	unsigned char TMPH,TMPL; 
	unsigned int temp;
	/*buf数组必须》=5B*/
	if(buf ==NULL)
			return;

	Reset();
	write_byte(jump_ROM);
	write_byte(start);
	Reset();
	write_byte(jump_ROM);
	write_byte(read_EEROM);
	TMPL = read_byte();
	TMPH = read_byte();
	temp = TMPL>>4 + TMPH<<4;

	buf[0] = temp/10 + '0';
	buf[1] = temp %10 + '0';
	buf[2] = '.';
	buf[3] = (char)(((TMPL&0x0F) * 0.625)%10) + '0';
	buf[4] = (char)(((TMPL&0x0F) * 6.25)%10) + '0';
	buf[5] = "\0";

	return;
}
#else

/**************************************
;WWW.STCMCU.COM
;Mobile:13922805190
;0755-82948412
温度传感器DS18B20测试程序
主芯片  : STC90C52RC (12T)
工作频率: 12.000MHz
**************************************/
BYTE TPH;                           //存放温度值的高字节
BYTE TPL;                           //存放温度值的低字节

void DelayXus(BYTE n);
void DS18B20_Reset();
void DS18B20_WriteByte(BYTE dat);
BYTE DS18B20_ReadByte();

void ds18b20_temp_get(char *buf)
{
	unsigned int temp;
	float decimals;

	DS18B20_Reset();                //设备复位
	DS18B20_WriteByte(0xCC);        //跳过ROM命令
	DS18B20_WriteByte(0x44);        //开始转换命令
	while (!DQ);                    //等待转换完成

	DS18B20_Reset();                //设备复位
	DS18B20_WriteByte(0xCC);        //跳过ROM命令
	DS18B20_WriteByte(0xBE);        //读暂存存储器命令
	TPL = DS18B20_ReadByte();       //读温度低字节
	TPH = DS18B20_ReadByte();       //读温度高字节
	temp =  TPL/16 + (TPH & 0x7)*16; /*关于左右位移 需要根据大小端决定*/
	decimals = (TPL & 0x0F) * 0.0625;
	
	/*buf数组必须》=6B*/
	buf[0] = temp / 10 + '0';
	buf[1] = temp % 10 + '0';
	buf[2] = '.';
	buf[3] = (unsigned int)(decimals * 10)%10 + '0';
	buf[4] = (unsigned int)(decimals * 100)%10 + '0';
	buf[5] = '\0';
	return;
}

/**************************************
延时X*10微秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数
当改用1T的MCU时,请调整此延时函数
**************************************/
void DelayX0us(BYTE n)
{
    while (n--)
    {
        _nop_();
        _nop_();
    }
}

/**************************************
复位DS18B20,并检测设备是否存在
**************************************/
void DS18B20_Reset()
{
    CY = 1;
    while (CY)
    {
        DQ = 0;                     //送出低电平复位信号
        DelayX0us(48);              //延时至少480us
        DQ = 1;                     //释放数据线
        DelayX0us(6);               //等待60us
        CY = DQ;                    //检测存在脉冲
        DelayX0us(42);              //等待设备释放数据线
    }
}

/**************************************
从DS18B20读1字节数据
**************************************/
BYTE DS18B20_ReadByte()
{
    BYTE i;
    BYTE dat = 0;

    for (i=0; i<8; i++)             //8位计数器
    {
        dat >>= 1;
        DQ = 0;                     //开始时间片
        _nop_();                    //延时等待
        _nop_();
        DQ = 1;                     //准备接收
        _nop_();                    //接收延时
        _nop_();
        if (DQ) dat |= 0x80;        //读取数据
        DelayX0us(6);               //等待时间片结束
    }

    return dat;
}

/**************************************
向DS18B20写1字节数据
**************************************/
void DS18B20_WriteByte(BYTE dat)
{
    char i;

    for (i=0; i<8; i++)             //8位计数器
    {
        DQ = 0;                     //开始时间片
        _nop_();                    //延时等待
        _nop_();
        dat >>= 1;                  //送出数据
        DQ = CY;
        DelayX0us(6);               //等待时间片结束
        DQ = 1;                     //恢复数据线
    }
}
#endif

#else/*DS18B20_MODULE*/
void ds18b20_temp_get(char *buf){}

#endif /*DS18B20_MODULE*/