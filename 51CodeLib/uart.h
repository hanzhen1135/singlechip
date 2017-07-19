#ifndef __UART_H__
#define __UART_H__

#include "module_config.h"

extern void uart_init();
extern void uart_send(char *buf);

#endif