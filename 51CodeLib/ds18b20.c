#include "ds18b20.h"

#ifdef DS18B20_MODULE

#if 0 /*dongliu*/

#define jump_ROM 0xCC
#define start 0x44
#define read_EEROM 0xBE

/********************************************************************
* ���� : delay()
* ���� : ��ʱ����
* ���� : ��
* ��� : ��
***********************************************************************/
void delay(uint N)
{
	int i;
	for(i=0; i<N; i++)
	;
}

/********************************************************************
* ���� : Reset()
* ���� : ��λDS18B20
* ���� : ��
* ��� : ��
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
* ���� : read_bit()
* ���� : ��DS18B20��һ��λֵ
* ���� : ��
* ��� : ��DS18B20������һ��λֵ
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
* ���� : write_bit()
* ���� : ��DS18B20дһλ
* ���� : bitval��Ҫ��DS18B20д���λֵ��
* ��� : ��
***********************************************************************/
void write_bit(uchar bitval)
{
	DQ=0;if(bitval==1)
	DQ=1;
	delay(5);
	DQ=1;
}

/********************************************************************
* ���� : read_byte()
* ���� : ��DS18B20��һ���ֽ�
* ���� : ��
* ��� : ��DS18B20������ֵ
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
* ���� : write_byte()
* ���� : ��DS18B20дһ���ֽ�
* ���� : val��Ҫ��DS18B20д�������ֵ��
* ��� : ��
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
* ���� : ds18b20_temp_get(char *buf)
* ���� : ��ȡds18b20�¶�
* ���� : val��Ҫ��DS18B20д�������ֵ��
* ��� : buf �¶��ַ���
******************************************************************/
void ds18b20_temp_get(char *buf)
{
	unsigned char TMPH,TMPL; 
	unsigned int temp;
	/*buf������롷=5B*/
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
�¶ȴ�����DS18B20���Գ���
��оƬ  : STC90C52RC (12T)
����Ƶ��: 12.000MHz
**************************************/
BYTE TPH;                           //����¶�ֵ�ĸ��ֽ�
BYTE TPL;                           //����¶�ֵ�ĵ��ֽ�

void DelayXus(BYTE n);
void DS18B20_Reset();
void DS18B20_WriteByte(BYTE dat);
BYTE DS18B20_ReadByte();

void ds18b20_temp_get(char *buf)
{
	unsigned int temp;
	float decimals;

	DS18B20_Reset();                //�豸��λ
	DS18B20_WriteByte(0xCC);        //����ROM����
	DS18B20_WriteByte(0x44);        //��ʼת������
	while (!DQ);                    //�ȴ�ת�����

	DS18B20_Reset();                //�豸��λ
	DS18B20_WriteByte(0xCC);        //����ROM����
	DS18B20_WriteByte(0xBE);        //���ݴ�洢������
	TPL = DS18B20_ReadByte();       //���¶ȵ��ֽ�
	TPH = DS18B20_ReadByte();       //���¶ȸ��ֽ�
	temp =  TPL/16 + (TPH & 0x7)*16; /*��������λ�� ��Ҫ���ݴ�С�˾���*/
	decimals = (TPL & 0x0F) * 0.0625;
	
	/*buf������롷=6B*/
	buf[0] = temp / 10 + '0';
	buf[1] = temp % 10 + '0';
	buf[2] = '.';
	buf[3] = (unsigned int)(decimals * 10)%10 + '0';
	buf[4] = (unsigned int)(decimals * 100)%10 + '0';
	buf[5] = '\0';
	return;
}

/**************************************
��ʱX*10΢��(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
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
��λDS18B20,������豸�Ƿ����
**************************************/
void DS18B20_Reset()
{
    CY = 1;
    while (CY)
    {
        DQ = 0;                     //�ͳ��͵�ƽ��λ�ź�
        DelayX0us(48);              //��ʱ����480us
        DQ = 1;                     //�ͷ�������
        DelayX0us(6);               //�ȴ�60us
        CY = DQ;                    //����������
        DelayX0us(42);              //�ȴ��豸�ͷ�������
    }
}

/**************************************
��DS18B20��1�ֽ�����
**************************************/
BYTE DS18B20_ReadByte()
{
    BYTE i;
    BYTE dat = 0;

    for (i=0; i<8; i++)             //8λ������
    {
        dat >>= 1;
        DQ = 0;                     //��ʼʱ��Ƭ
        _nop_();                    //��ʱ�ȴ�
        _nop_();
        DQ = 1;                     //׼������
        _nop_();                    //������ʱ
        _nop_();
        if (DQ) dat |= 0x80;        //��ȡ����
        DelayX0us(6);               //�ȴ�ʱ��Ƭ����
    }

    return dat;
}

/**************************************
��DS18B20д1�ֽ�����
**************************************/
void DS18B20_WriteByte(BYTE dat)
{
    char i;

    for (i=0; i<8; i++)             //8λ������
    {
        DQ = 0;                     //��ʼʱ��Ƭ
        _nop_();                    //��ʱ�ȴ�
        _nop_();
        dat >>= 1;                  //�ͳ�����
        DQ = CY;
        DelayX0us(6);               //�ȴ�ʱ��Ƭ����
        DQ = 1;                     //�ָ�������
    }
}
#endif

#else/*DS18B20_MODULE*/
void ds18b20_temp_get(char *buf){}

#endif /*DS18B20_MODULE*/