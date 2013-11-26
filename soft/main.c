#include "simple_uart.h"
#include "boards.h"
#include "nrf_delay.h"

int main(void)
{
    simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, false);
    simple_uart_putstring((const uint8_t *)" \nStart:\n");

    while(true)
    {
        uint8_t cr = simple_uart_get();

        if (cr >= 'A' && cr <= 'z') {
            simple_uart_put(cr);
            simple_uart_put('\n');

            if(cr == 'q' || cr == 'Q')
            {
                simple_uart_putstring((const uint8_t *)" \nExit!\n");
                for(;;);
            }
        }
        nrf_delay_ms(10);
    }
}

