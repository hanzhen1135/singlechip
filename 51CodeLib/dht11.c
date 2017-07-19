
#include "dht11.h"

#ifdef DHT11_MODULE

typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  无符号8位整型变量  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  有符号8位整型变量  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  无符号16位整型变量 */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  有符号16位整型变量 */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  无符号32位整型变量 */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  有符号32位整型变量 */
typedef float          F32;      /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         F64;      /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */

U8  U8FLAG;
U8  U8temp;
U8  U8comdata;

static void Delay(U16 j)
{ 
	U8 i;
	for(;j>0;j--)
	{ 	
		for(i=0;i<27;i++);
	}
}
static void  Delay_10us(void)
{
	U8 i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}
void rev_data(void)
{

	U8 i;

	for(i=0;i<8;i++)	   
	{
		U8FLAG=2;	
		while((!DHT_DQ)&&U8FLAG++);
		Delay_10us();
		Delay_10us();
		Delay_10us();
		U8temp=0;
		if(DHT_DQ)U8temp=1;
		U8FLAG=2;

		while((DHT_DQ)&&U8FLAG++);
		//超时则跳出for循环		  
		if(U8FLAG==1)break;
		//判断数据位是0还是1	 

		// 如果高电平高过预定0高电平值则数据位为 1 

		U8comdata<<=1;
		U8comdata|=U8temp;        //0
	}//rof
}

//--------------------------------
//-----湿度读取子程序 ------------
//--------------------------------
//----以下变量均为全局变量--------
//----温度高8位== U8T_data_H------
//----温度低8位== U8T_data_L------
//----湿度高8位== U8RH_data_H-----
//----湿度低8位== U8RH_data_L-----
//----校验 8位 == U8checkdata-----
//----调用相关子程序如下----------
//---- Delay();, Delay_10us();,rev_data(); 
//--------------------------------
void RH_T_get(char *RH_buf, char *T_buf)
{
	U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
	//主机拉低18ms 
	 DHT_DQ=0;
	 Delay(180);
	 DHT_DQ=1;
 //总线由上拉电阻拉高 主机延时20us
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
 //主机设为输入 判断从机响应信号 
	 DHT_DQ=1;
 //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	 if(!DHT_DQ)		 //T !	  
	 {
		 U8FLAG=2;
	 //判断从机是否发出 80us 的低电平响应信号是否结束	 
		 while((!DHT_DQ)&&U8FLAG++);
		 U8FLAG=2;
	 //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		 while((DHT_DQ)&&U8FLAG++);
	 //数据接收状态		 
		 rev_data();
		 U8RH_data_H=U8comdata;
		 rev_data();
		 U8RH_data_L=U8comdata;
		 rev_data();
		 U8T_data_H=U8comdata;
		 rev_data();
		 U8T_data_L=U8comdata;
		 rev_data();
		 U8checkdata=U8comdata;
		 DHT_DQ=1;

	 //数据校验 
		 if((U8T_data_H+U8T_data_L+U8RH_data_H+U8RH_data_L)!=U8checkdata)
		 {
				U8RH_data_H=0;
				U8RH_data_L=0;
				U8T_data_H=0;
				U8T_data_L=0;
				U8checkdata=0;
		 }
		 RH_buf[0] = U8RH_data_H/10 + '0';
		 RH_buf[1] = U8RH_data_H%10 + '0';
		 RH_buf[2] = '\0';
		 
		 T_buf[0] = U8T_data_H/10 + '0';
		 T_buf[1] = U8T_data_H%10 + '0';
		 T_buf[2] = '\0';
		 
	}
	return;
}
#else /*DHT11_MODULE*/

void RH_T_get(char *RH_buf, char *T_buf){}

#endif /*DHT11_MODULE*/