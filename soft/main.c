#include "board.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_esb.h"

#define PIPE_NUMBER 0
#define TX_PAYLOAD_LENGTH 2

// Data and acknowledgement payloads
static uint8_t my_tx_payload[TX_PAYLOAD_LENGTH];                ///< Payload to send to PRX.
static uint8_t my_rx_payload[NRF_ESB_CONST_MAX_PAYLOAD_LENGTH]; ///< Placeholder for received ACK payloads from PRX.

/////////////////////////////////////////////////////////////////////////////
// main
/////////////////////////////////////////////////////////////////////////////

int main(void)
{
    // Initialize GPIOs
    nrf_gpio_cfg_output(LED);

    // Initialize ESB
    nrf_esb_init(NRF_ESB_MODE_PTX);
    // Mostly default parameters, preciser for clarity with RX
    {
        nrf_esb_set_xosc_ctl(NRF_ESB_XOSC_CTL_AUTO);
        nrf_esb_set_crc_length(NRF_ESB_CRC_LENGTH_1_BYTE);
        nrf_esb_set_datarate(NRF_ESB_DATARATE_2_MBPS);
        nrf_esb_set_retransmit_delay(600); // because 2_MBPS
        nrf_esb_set_max_number_of_tx_attempts(0); // unlimited
        nrf_esb_set_enabled_prx_pipes(NRF_ESB_DEFAULT_ENABLED_PRX_PIPES);
        nrf_esb_set_base_address_length(NRF_ESB_BASE_ADDRESS_LENGTH_4B);
        nrf_esb_set_address_prefix_byte(0, 0x66);
        nrf_esb_set_base_address_0(0x66666666);
        nrf_esb_set_channel(1);
        nrf_esb_enable_dyn_ack();
//        nrf_esb_set_output_power(NRF_ESB_OUTPUT_POWER_4_DBM); // default 0
    }
	// Add packet into TX queue
    nrf_esb_add_packet_to_tx_fifo(PIPE_NUMBER, my_tx_payload, TX_PAYLOAD_LENGTH, NRF_ESB_PACKET_USE_ACK);
    nrf_esb_enable();

    while(1)
    {
        // Optionally set the CPU to sleep while waiting for a callback.
        // __WFI();
    }
}


/////////////////////////////////////////////////////////////////////////////
// ESB callback function definitions
/////////////////////////////////////////////////////////////////////////////


// If an ACK was received, we send another packet.
void nrf_esb_tx_success(uint32_t tx_pipe, int32_t rssi) {
    my_tx_payload[0]++;
    nrf_gpio_pin_write(LED, my_tx_payload[0] & 1);
    nrf_esb_add_packet_to_tx_fifo(PIPE_NUMBER, my_tx_payload, TX_PAYLOAD_LENGTH, NRF_ESB_PACKET_USE_ACK);

    nrf_delay_ms(150);
}


// If the transmission failed, resend the last packet.
void nrf_esb_tx_failed(uint32_t tx_pipe) {
    nrf_esb_add_packet_to_tx_fifo(PIPE_NUMBER, my_tx_payload, TX_PAYLOAD_LENGTH, NRF_ESB_PACKET_USE_ACK);
    nrf_gpio_pin_set(LED);
    nrf_delay_ms(1000);
}

// Reception hanfler - not useful for the moment
void nrf_esb_rx_data_ready(uint32_t rx_pipe, int32_t rssi) {
    uint32_t my_rx_payload_length;
    // Pop packet and write first byte of the payload to the GPIO pin.
    nrf_esb_fetch_packet_from_rx_fifo(PIPE_NUMBER, my_rx_payload, &my_rx_payload_length);
    if (my_rx_payload_length > 0)
    {
        // TODO: use nrf_esb_get_rx_fifo_packet_count (uint32_t pipe)
        nrf_gpio_pin_toggle(LED);
    }

    // Flushing Rx fifo so that there is space for future transmits
//  nrf_esb_flush_rx_fifo(PIPE_NUMBER); // TODO ?
}

// Called after the nrf_esb_disable() process is done, to sleep for example
void nrf_esb_disabled(void) {}

