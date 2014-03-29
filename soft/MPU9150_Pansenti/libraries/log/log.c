#include "log.h" // TODO rename to tools.h

#ifdef DEBUG_PRINT
void simple_uart_print_int(int i)
{
    char buf[12]; // 2**32 = 4G => sign + 10 numbers + '\0'
    sprintf(buf, "%d" , i);
    simple_uart_putstring((const uint8_t *)buf);
}
#endif

