#include"uart.h"
#ifdef UART_MODULE



/*TODO:波特率初始化 可优化*/
void uart_init()
{
#if 0
	SCON = 0x50;       //REN=1允许串行接受状态，串口工作模式2     	   
	TMOD|= 0x20;      //定时器工作方式2                       
	PCON|= 0x80;      //波特率提高一倍                                                    
		 
	TH1 = 0xF3;					// //baud*2  /*  波特率4800、数据位8、停止位1。效验位无 (12M)
	TL1 = 0xF3; 
	TR1  = 1;        //开启定时器1                                                      
	ES   = 1;        //开串口中断                  
	EA   = 1;        // 开总中断 
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
		while(!TI)                   //如果发送完毕，硬件会置位TI
		{
			_nop_();	
		}
		p++;
		TI = 0;		                //TI清零
	}
}

#else /*UART_MODULE*/

void uart_init(){}
void uart_send(char *buf){}

#endif /*UART_MODULE*/