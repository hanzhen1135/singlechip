
#include "dht11.h"

#ifdef DHT11_MODULE

typedef unsigned char  U8;       /* defined for unsigned 8-bits integer variable 	  �޷���8λ���ͱ���  */
typedef signed   char  S8;       /* defined for signed 8-bits integer variable		  �з���8λ���ͱ���  */
typedef unsigned int   U16;      /* defined for unsigned 16-bits integer variable 	  �޷���16λ���ͱ��� */
typedef signed   int   S16;      /* defined for signed 16-bits integer variable 	  �з���16λ���ͱ��� */
typedef unsigned long  U32;      /* defined for unsigned 32-bits integer variable 	  �޷���32λ���ͱ��� */
typedef signed   long  S32;      /* defined for signed 32-bits integer variable 	  �з���32λ���ͱ��� */
typedef float          F32;      /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         F64;      /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */

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
		//��ʱ������forѭ��		  
		if(U8FLAG==1)break;
		//�ж�����λ��0����1	 

		// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 

		U8comdata<<=1;
		U8comdata|=U8temp;        //0
	}//rof
}

//--------------------------------
//-----ʪ�ȶ�ȡ�ӳ��� ------------
//--------------------------------
//----���±�����Ϊȫ�ֱ���--------
//----�¶ȸ�8λ== U8T_data_H------
//----�¶ȵ�8λ== U8T_data_L------
//----ʪ�ȸ�8λ== U8RH_data_H-----
//----ʪ�ȵ�8λ== U8RH_data_L-----
//----У�� 8λ == U8checkdata-----
//----��������ӳ�������----------
//---- Delay();, Delay_10us();,rev_data(); 
//--------------------------------
void RH_T_get(char *RH_buf, char *T_buf)
{
	U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
	//��������18ms 
	 DHT_DQ=0;
	 Delay(180);
	 DHT_DQ=1;
 //������������������ ������ʱ20us
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
 //������Ϊ���� �жϴӻ���Ӧ�ź� 
	 DHT_DQ=1;
 //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	 if(!DHT_DQ)		 //T !	  
	 {
		 U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
		 while((!DHT_DQ)&&U8FLAG++);
		 U8FLAG=2;
	 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
		 while((DHT_DQ)&&U8FLAG++);
	 //���ݽ���״̬		 
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

	 //����У�� 
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