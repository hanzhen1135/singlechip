
#ifndef __DS18B20_C__
#define __DS18B20_C__

#include "module_config.h"

/*���ö˿�*/
sbit DQ = P2^3;            //DS18B20���ݶ˿�

/*��ȡ��Χ100< temp < 0*/
/*buf����ռ���� >= 6B*/
extern void ds18b20_temp_get(char *buf);

#endif