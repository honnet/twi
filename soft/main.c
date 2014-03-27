#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "nrf_gpio.h"
#include "board.h"
#include "nrf_delay.h"
#include "MPU9150.h"

void simple_uart_print_int(int i);

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#  include <stdlib.h> // sprintf()
#  include "simple_uart.h"
#  define DP_cnf(...) simple_uart_config(__VA_ARGS__)
#  define DP_int(x)   simple_uart_print_int(x)
#  define DP_str(x)   simple_uart_putstring((const uint8_t *)(x))
#else
#  define DP_cnf(...)
#  define DP_int(x)
#  define DP_str(x)
#endif

MPU9150 accelGyroMag;
int16_t a[3], g[3], m[3];

void init(void)
{
    nrf_gpio_cfg_output(LED);
    DP_cnf(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, false);
    accelGyroMag.initialize();
    DP_str( accelGyroMag.testConnection() ?
            "MPU9150 connection OK\r\n" :
            "MPU9150 connection failed\r\n" );
}

int main(void)
{
    char b = 0;
    init();

    while(1)
    {
        accelGyroMag.getMotion9(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2], &m[0], &m[1], &m[2]);
/*
        // accelerometer:
        for (int i=0; i<3; i++) {
            DP_int(m[i]);
            DP_str("\t");
        }
*/
        // heading:
        float heading = atan2(m[1], m[0]); // atan2(my, mx);
        if(heading < 0)
            heading += 2 * M_PI;
        heading *= 180 / M_PI;
        DP_int(heading);
        DP_str("\t");

        for (int i = 0; i < 90; i++)
        {
            if (i < heading / 4) // max = 360/4 = 90
                DP_str("+");
            else
                DP_str(" ");
        }
        DP_str("|\r\n");

        nrf_gpio_pin_write(LED, b = !b);
        nrf_delay_ms(10);
    }
}

void simple_uart_print_int(int i)
{
    char buf[12]; // 2**32 = 4G => sign + 10 numbers + '\0'
    sprintf(buf, "%d" , i);
    simple_uart_putstring((const uint8_t *)buf);
}
