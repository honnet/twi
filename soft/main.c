/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
*
* @defgroup nrf_dev_led_radio_tx_example_main main.c
* @{
* @ingroup nrf_dev_led_radio_tx_example
*
* @brief Radio Transmitter Example Application main file.
*
* This file contains the source code for a sample application using the NRF_RADIO peripheral.
*
*/

#include <stdio.h>
#include <stdint.h>
#include "radio_config.h"
#include "nrf_gpio.h"
#include "board.h"
#include "nrf_delay.h"
#include "simple_uart.h"
#include "MPU9150.h"

static uint8_t packet[4];  ///< Packet to transmit

MPU9150 accelGyroMag;
int16_t a[3], g[3], m[3];

void init(void)
{
  nrf_gpio_cfg_output(LED);

  simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, false);
  simple_uart_putstring((const uint8_t *)"\r\nStarting IMU...\r\n");

  accelGyroMag.initialize();
  simple_uart_putstring( accelGyroMag.testConnection() ?
                        (const uint8_t *)"MPU9150 connection successful\r\n" :
                        (const uint8_t *)"MPU9150 connection failed\r\n" );

///* Start 16 MHz crystal oscillator */
//NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
//NRF_CLOCK->TASKS_HFCLKSTART = 1;

///* Wait for the external oscillator to start up */
//while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
//{
//}

//// Set radio configuration parameters
//radio_configure();

//// Set payload pointer
//NRF_RADIO->PACKETPTR = (uint32_t)packet;
}

/**
 * @brief Function for application main entry.
 * @return 0. int return type required by ANSI/ISO standard.
 */

#include "I2Cdev.h"

int main(void)
{
    init();
    uint8_t devAddr = 0x68;

    char b = 0;
    while(1)
    {
        uint8_t val;
        char buf[16];
        for (uint16_t reg=0; reg<=0xFF; reg++) {
            I2Cdev::readBuf(devAddr<<1, reg, &val, 1);
            if (val)
            {
                sprintf(buf, "%x = %x\n", reg, val);
                simple_uart_putstring((const uint8_t *)buf);
            }
        }
        simple_uart_putstring((const uint8_t *)"\n");

    /*

    accelGyroMag.getMotion9(&a[0], &a[1], &a[2], &g[0], &g[1], &g[2], &m[0], &m[1], &m[2]);
    char buf[8]; // max = 2**16 = 65536
    for (int i=0; i<3; i++) {
        sprintf(buf, "%d\t" , a[i]);
        simple_uart_putstring((const uint8_t *)buf);
    }
    for (int i=0; i<3; i++) {
        sprintf(buf, "%d\t" , g[i]);
        simple_uart_putstring((const uint8_t *)buf);
    }
    for (int i=0; i<3; i++) {
        sprintf(buf, "%d\t" , m[i]);
        simple_uart_putstring((const uint8_t *)buf);
    }
    simple_uart_putstring((const uint8_t *)"\n");
*/
    if (b)
      nrf_gpio_pin_set(LED);
    else
      nrf_gpio_pin_clear(LED);
/*
    // Place the read character in the payload, enable the radio and
    // send the packet:
    packet[0] = b ? '1' : '0';
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
*/
    b = !b;
    nrf_delay_ms(90);
  }
}

/**
 *@}
 **/
