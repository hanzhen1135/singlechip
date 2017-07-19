#ifndef __DHT11_H__
#define __DHT11_H__

#include "module_config.h"


//----------------------------------------------//
//----------------IO口定义区--------------------//
//----------------------------------------------//
sbit  DHT_DQ  = P2^0 ;

/*DHT11 range:20-90%RH 0-50T accuracy: +-5%RH +-2T*/

extern void RH_T_get(char *RH_buf, char *T_buf);

#endif