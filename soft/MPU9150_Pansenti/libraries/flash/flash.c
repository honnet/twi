#include <stdbool.h>
#include "nrf.h"
#include "nrf51.h"
#include "nrf_gpio.h"


/** @brief Function for erasing a page in flash.
 *
 * @param page_address Address of the first word in the page to be erased.
 */
void flash_page_erase(uint32_t *page_address)
{
  // Turn on flash erase enable and wait until the NVMC is ready:
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Een << NVMC_CONFIG_WEN_Pos);
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    {
        // Do nothing.
    }

    // Erase page:
    NRF_NVMC->ERASEPAGE = (uint32_t)page_address;

    while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    {
        // Do nothing.
    }

    // Turn off flash erase enable and wait until the NVMC is ready:
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);

    while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    {
        // Do nothing.
    }
}


/** @brief Function for filling a page in flash with a value.
 *
 * @param[in] address Address of the first word in the page to be filled.
 * @param[in] value Value to be written to flash.
 */
void flash_word_write(uint32_t *address, uint32_t value)
{
    // Turn on flash write enable and wait until the NVMC is ready:
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);

    while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    {
        // Do nothing.
    }

    *address = value;

    while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    {
        // Do nothing.
    }

    // Turn off flash write enable and wait until the NVMC is ready:
    NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);

    while (NRF_NVMC->READY == NVMC_READY_READY_Busy)
    {
        // Do nothing.
    }
}

/*
int main(void)
{
    uint32_t *addr;
    uint8_t patwr;
    uint8_t patrd;
    uint8_t patold;
    uint32_t i;
    uint32_t pg_size;
    uint32_t pg_num;

    patold  = 0;
    pg_size = NRF_FICR->CODEPAGESIZE;
    pg_num  = NRF_FICR->CODESIZE - 1;    // Use last page in flash

    while (true)
    {
        // Start address:
        addr = (uint32_t *)(pg_size * pg_num);
        // Erase page:
        flash_page_erase(addr);
        i = 0;
        do
        {
            // Read pattern from port 0 (pins0-7), and write it to flash:
            patwr =  nrf_gpio_port_read(NRF_GPIO_PORT_SELECT_PORT0);
            if (patold != patwr)
            {
                patold = patwr;
                flash_word_write(++addr, (uint32_t)patwr);
                i += 4;
            }
            // Read pattern from flash and write it to port 1 (pins8-15):
            patrd = (uint8_t)*addr;
            nrf_gpio_port_write(NRF_GPIO_PORT_SELECT_PORT1, patrd);
        } while (i < pg_size);
    }
}
*/

