#include"uart.h"
#ifdef UART_MODULE



/*TODO:�����ʳ�ʼ�� ���Ż�*/
void uart_init()
{
#if 0
	SCON = 0x50;       //REN=1�����н���״̬�����ڹ���ģʽ2     	   
	TMOD|= 0x20;      //��ʱ��������ʽ2                       
	PCON|= 0x80;      //���������һ��                                                    
		 
	TH1 = 0xF3;					// //baud*2  /*  ������4800������λ8��ֹͣλ1��Ч��λ�� (12M)
	TL1 = 0xF3; 
	TR1  = 1;        //������ʱ��1                                                      
	ES   = 1;        //�������ж�                  
	EA   = 1;        // �����ж� 
	// IE = 0x0;
#else
/*9600*/
	TMOD |= 0x20;
	PCON = 0x00;
	SCON = 0x50;			
	TH1 = 0xFd;
	TL1 = 0xFd;
	TR1 = 1;		

#endif
	
	return;
}

void uart_send(char *buf)
{
	char *p = buf;
	while(*p != '\0')
	{
		SBUF = *p;
		while(!TI)                   //���������ϣ�Ӳ������λTI
		{
			_nop_();	
		}
		p++;
		TI = 0;		                //TI����
	}
}

#else /*UART_MODULE*/

void uart_init(){}
void uart_send(char *buf){}

#endif /*UART_MODULE*/