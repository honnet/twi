#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "nrf_gpio.h"
#include "board.h"
#include "nrf_delay.h"
#include "MPU9150.h"
#include "radio_config.h"

void simple_uart_print_int(int i);

//#define DEBUG_PRINT
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

void radio_send(uint8_t data)
{
    my_tx_payload[0] = data;
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
enum RGB {
    R = AIN0,
    G = AIN1,
    B = 1 // ex-uart rx
};

void rgb_led(uint8_t led, bool on)
{
    if (on)
    {
        nrf_gpio_cfg_output(led);
        nrf_gpio_pin_clear(led);
    }
    else
    {
        nrf_gpio_cfg_input(led, NRF_GPIO_PIN_NOPULL);
    }
}

void init(void)
{
    nrf_gpio_cfg_output(LED);
    nrf_gpio_cfg_input(BUTTON, NRF_GPIO_PIN_PULLDOWN);

    DP_cnf(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, false);
    DP_str("Starting IMU:\n");

    accelGyroMag.initialize();
    DP_str( accelGyroMag.testConnection() ?
            "MPU9150 connection OK\r\n" :
            "MPU9150 connection failed\r\n" );

    // Initialize radio
    radio_init();
}

/////////////////////////////////////////////////////////////////////////////

uint8_t poll_sensors(void) {
    const int8_t threshold = 24; // note: [-128, 127] <=> [-2g, 2g]

    uint8_t bitmap = 0;

    int8_t x = accelGyroMag.getAccelerationX() >> 8;
    int8_t z = accelGyroMag.getAccelerationZ() >> 8;

    if (x > threshold)       bitmap |= 1 << 0; // red
    else if (x < -threshold) bitmap |= 1 << 1; // yellow (G+R)

    if (z > threshold)       bitmap |= 1 << 2; // blue
    else if (z < -threshold) bitmap |= 1 << 3; // green

    if (nrf_gpio_pin_read(BUTTON)) bitmap |= 1 << 4;

    return bitmap;
}

void actuate(uint8_t bitmap) {
    if (bitmap & 1<<0) { // R
        rgb_led(R, 1);
        rgb_led(G, 0);
        rgb_led(B, 0);
    }
    else if (bitmap & 1<<1) { // yellow
        rgb_led(R, 1);
        rgb_led(G, 1);
        rgb_led(B, 0);
    }
    else if (bitmap & 1<<2) { // B
        rgb_led(R, 0);
        rgb_led(G, 0);
        rgb_led(B, 1);
    }
    else if (bitmap & 1<<3) { // G
        rgb_led(R, 0);
        rgb_led(G, 1);
        rgb_led(B, 0);
    }
    else {                    // off
        rgb_led(R, 0);
        rgb_led(G, 0);
        rgb_led(B, 0);
    }
}


/////////////////////////////////////////////////////////////////////////////

int main(void)
{
    init();

    while(1)
    {
        uint8_t bitmap = poll_sensors();
        radio_send(bitmap);

        actuate(bitmap);

        nrf_gpio_pin_set(LED);
        nrf_delay_ms(1); // just enough to know if we're frozen
        nrf_gpio_pin_clear(LED);
        nrf_delay_ms(49);
    }
}

/////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG_PRINT
void simple_uart_print_int(int i)
{
    char buf[12]; // 2**32 = 4G => sign + 10 numbers + '\0'
    sprintf(buf, "%d" , i);
    simple_uart_putstring((const uint8_t *)buf);
}
#endif

