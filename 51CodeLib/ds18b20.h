
#ifndef __DS18B20_C__
#define __DS18B20_C__

#include "module_config.h"

/*配置端口*/
sbit DQ = P2^3;            //DS18B20数据端口

/*获取范围100< temp < 0*/
/*buf数组空间必须 >= 6B*/
extern void ds18b20_temp_get(char *buf);

#endif