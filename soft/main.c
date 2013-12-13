#include "simple_uart.h"
#include "board.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "twi_master.h"
#include "mpu6050.h"

static const int DEVADR = 0x68;

#define STRINGIFY(x) #x
#define STR(x) STRINGIFY(x)
#pragma message "\n\n>>> TX PIN = " STR(TX_PIN_NUMBER)
#pragma message "\n\n>>> RX PIN = " STR(RX_PIN_NUMBER)
#include "twi_master_config.h"
#pragma message "\n\n>>> SDA PIN = " STR(TWI_MASTER_CONFIG_DATA_PIN_NUMBER)
#pragma message "\n\n>>> SCL PIN = " STR(TWI_MASTER_CONFIG_CLOCK_PIN_NUMBER)

int main(void)
{
    nrf_gpio_cfg_input(BUTTON, NRF_GPIO_PIN_PULLDOWN);
    nrf_gpio_cfg_output(LED);
    nrf_gpio_pin_set(LED);

    simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, false);
    simple_uart_putstring((const uint8_t *)" \r\nStart:\r\n");

    // initialize I2C:
    if (!twi_master_init())
        for(;; nrf_gpio_pin_toggle(LED), nrf_delay_ms(50));     // quick blink if error

    // initialize IMU:
    nrf_delay_ms(4); // time needed to power up the IMU
    if (!mpu6050_init(DEVADR))
        for(;; nrf_gpio_pin_toggle(LED), nrf_delay_ms(150));    // less quick blink if error

    while(true) {
        const uint8_t to = 10; // ms
        uint8_t cr = '0';
        if ( simple_uart_get_with_timeout(to, &cr) ) {
            if (cr >= 'A' && cr <= 'z') {
                simple_uart_putstring((const uint8_t *)"Received: ");
                simple_uart_put(cr);
                simple_uart_putstring((const uint8_t *)"\r\n");
            }
        }

        if (nrf_gpio_pin_read(BUTTON) ) {
            simple_uart_putstring((const uint8_t *)"Button pressed.\r\n");

            for (int i=0; i<4; i++) {
                nrf_gpio_pin_toggle(LED);
                nrf_delay_ms(100);
            }
        }
    }
}

