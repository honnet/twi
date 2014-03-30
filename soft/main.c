#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "nrf_gpio.h"
#include "board.h"
#include "nrf_delay.h"
#include "MPU9150.h"
#include "radio_config.h"

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

// Data and acknowledgement payloads
static uint8_t my_tx_payload[PACKET_STATIC_LENGTH];

MPU9150 accelGyroMag;

/////////////////////////////////////////////////////////////////////////////

void radio_init(void)
{
    /* Start 16 MHz crystal oscillator */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;

    /* Wait for the external oscillator to start up */
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) { }

    // Set radio configuration parameters
    radio_configure();

    // Set payload pointer
    NRF_RADIO->PACKETPTR = (uint32_t)my_tx_payload;
}

void radio_send()
{
    // send the packet:
    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN = 1;
    while (NRF_RADIO->EVENTS_READY == 0U)
    {
    }
    NRF_RADIO->TASKS_START = 1U;
    NRF_RADIO->EVENTS_END = 0U;
    while(NRF_RADIO->EVENTS_END == 0U)
    {
    }
    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;
    while(NRF_RADIO->EVENTS_DISABLED == 0U)
    {
    }
}

/////////////////////////////////////////////////////////////////////////////

void init(void)
{
    nrf_gpio_cfg_output(LED);
    nrf_gpio_pin_set(LED);
    nrf_gpio_cfg_input(BUTTON, NRF_GPIO_PIN_PULLDOWN);

    DP_cnf(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, false);
    DP_str("Starting IMU:\n");

    accelGyroMag.initialize();
    DP_str( accelGyroMag.testConnection() ?
            "MPU9150 connection OK\r\n" :
            "MPU9150 connection failed\r\n" );

    // Initialize ESB
    radio_init();
}

/////////////////////////////////////////////////////////////////////////////

uint8_t poll_sensors(void) {
    const int8_t threshold = 1 << 5; // 2**5 = 0.5g because [-2**7, 2**7] = [-2g, 2g]

//    static uint8_t old_bitmap = 0; // button(MSB), -y, y, -x, x (LSB)
    uint8_t bitmap = 0;

    int8_t x = accelGyroMag.getAccelerationX() >> 8;
    int8_t y = accelGyroMag.getAccelerationY() >> 8;

    if (x > threshold)       bitmap |= 1 << 0;
    else if (x < -threshold) bitmap |= 1 << 1;

    if (y > threshold)       bitmap |= 1 << 2;
    else if (y < -threshold) bitmap |= 1 << 3;

    if (nrf_gpio_pin_read(BUTTON)) bitmap |= 1 << 4;

    DP_int(bitmap);
    DP_str("\t");

    DP_int(y);
    DP_str("\t");
    for (int i = 0; i < 64; i++) { // 16bit => 6bits (max = 63, min = -64)
        if ( i < abs(y>>1) ) DP_str("y");
        else                     DP_str(" ");
    }
    DP_str("|\r\n");

    if (1) // old_bitmap && old_bitmap != bitmap)
        return y;// bitmap;
    else
        return 0;
}

/////////////////////////////////////////////////////////////////////////////
volatile bool ready_flag = 1;
volatile bool error_flag = 0;

int main(void)
{
    init();

    while(1)
    {
        my_tx_payload[0] = poll_sensors();
        radio_send();

        nrf_gpio_pin_toggle(LED);
        nrf_delay_ms(50);
    }
}

/////////////////////////////////////////////////////////////////////////////

void simple_uart_print_int(int i)
{
    char buf[12]; // 2**32 = 4G => sign + 10 numbers + '\0'
    sprintf(buf, "%d" , i);
    simple_uart_putstring((const uint8_t *)buf);
}

