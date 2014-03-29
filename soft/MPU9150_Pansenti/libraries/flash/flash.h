#ifndef _FLASH_H
#define _FLASH_H

#include <stdint.h>

/** @brief Function for erasing a page in flash.
 *
 * @param page_address Address of the first word in the page to be erased.
 */
void flash_page_erase(uint32_t *page_address);


/** @brief Function for filling a page in flash with a value.
 *
 * @param[in] address Address of the first word in the page to be filled.
 * @param[in] value Value to be written to flash.
 */
void flash_word_write(uint32_t *address, uint32_t value);

#endif // _FLASH_H

