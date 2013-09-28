// simple blink test (on all pins)

#include "nrf_delay.h"
#include "nrf_gpio.h"

int main(void) {
    nrf_gpio_range_cfg_output(0,31);            // set all pins as output
    uint8_t reg_out8 = 0;                       // set all pins at 0

    for(;;) {
        int i;
        for (i = 0; i < 4; i++)
            nrf_gpio_port_write(i, reg_out8);   // set port i bits
        reg_out8 = ~reg_out8;                   // toggle all bits

        nrf_delay_ms(50);
    }
}
