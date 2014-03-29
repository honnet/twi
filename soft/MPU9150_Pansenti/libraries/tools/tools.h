#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "nrf_gpio.h"
#include "board.h"

#include "nrf_delay.h"
#define delay_ms(x) nrf_delay_ms(x)
#define delay(x)    nrf_delay_ms(x)


#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#  include <stdio.h>
#  include "simple_uart.h"
void simple_uart_print_int(int i);
#  define DP_cnf(...)   {simple_uart_config(__VA_ARGS__); delay(5);}
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

