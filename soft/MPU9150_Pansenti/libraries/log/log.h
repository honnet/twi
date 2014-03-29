#ifndef _LOG_H_  // TODO rename to tools.c
#define _LOG_H_

#include "nrf_delay.h"
#define delay_ms nrf_delay_ms

#define DEBUG_PRINT

void simple_uart_print_int(int i);

#ifdef DEBUG_PRINT
#  include <stdio.h>
#  include "simple_uart.h"
#  define DP_cnf(...)   simple_uart_config(__VA_ARGS__)
#  define DP_str(x)     simple_uart_putstring((const uint8_t *)(x))
#  define DP_int(x)     simple_uart_print_int(x)
#  define DP_int_ln(x)  {simple_uart_print_int(x); DP_str("\n");}
#else
#  define DP_cnf(...)
#  define DP_str(x)
#  define DP_int(x)
#  define DP_int_ln(x)
#endif

#endif // _MAIN_H_

