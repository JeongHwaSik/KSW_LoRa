#ifndef SCAN_RAK_FLASH_H
#define SCAN_RAK_FLASH_H

// INCLUDE EXTERNAL LIBRARIES
#include <hardware/flash.h>

// DEFINE LOCAL CONSTANTS
#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)

// FLASH LOCATION MARKER
#define LORAWAN_DATA_MARKER 0x55

// DECLARE LOCAL FUNCTIONS
void make_credentials();
void init_flash();
bool save_settings();
void flash_reset();
void log_settings();

#endif //SCAN_RAK_FLASH_H
