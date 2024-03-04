#include "scanin.h"

/**
 * @brief   Generate LoRaWAN credentials based on the unique device flash ID.
 *
 * @details This function reads the unique device ID of the RP2040, and
 *          then maps this ID to various LoRaWAN credentials such as
 *          DevEUI, AppEUI, AppKey, NwsKey, AppsKey, and DevAddr.
 *
 * @note    No input parameters.
 * @retval  None.
 *
 * @author  Khalifa Alremeithi <kalremei@purdue.edu>
 */
void make_credentials()
{
    uint8_t pico_id[8];

    // Get flash ID
    getUniqueDeviceID(pico_id);

#if APP_DEBUG > 0
    Serial.print("RP2040 Flash ID: ");
    for (unsigned char idx : pico_id)
    {
        Serial.printf("%02X", idx);
    }
    Serial.println("");
#endif

    // Create LoRaWAN credentials from flash ID
    int idx_1 = 0;
    int idx_2 = 7;
    for (unsigned char idx : pico_id)
    {
        g_lorawan_settings.node_device_eui[idx_1] = idx;
        g_lorawan_settings.node_app_eui[idx_2] = idx;
        idx_1++;
        idx_2--;
    }

    idx_1 = 0;
    idx_2 = 15;
    for (unsigned char idx : pico_id)
    {
        g_lorawan_settings.node_app_key[idx_1] = idx;
        g_lorawan_settings.node_app_key[idx_2] = idx;
        g_lorawan_settings.node_nws_key[idx_1] = idx;
        g_lorawan_settings.node_nws_key[idx_2] = idx;
        g_lorawan_settings.node_apps_key[idx_2] = idx;
        g_lorawan_settings.node_apps_key[idx_1] = idx;
        idx_1++;
        idx_2--;
    }

    g_lorawan_settings.node_dev_addr =
        ((pico_id[0] + pico_id[1]) << 24) | ((pico_id[2] + pico_id[3]) << 18) | ((pico_id[4] + pico_id[5]) << 8) |
        ((pico_id[6] + pico_id[7]));

#if APP_DEBUG > 0
    Serial.print("DevEUI = ");
    for (unsigned char idx : g_lorawan_settings.node_device_eui)
    {
        Serial.printf("%02X", idx);
    }
    Serial.print("\n\nAppEUI = ");
    for (unsigned char idx : g_lorawan_settings.node_app_eui)
    {
        Serial.printf("%02X", idx);
    }
    Serial.print("\n\nAppKey = ");
    for (unsigned char idx : g_lorawan_settings.node_app_key)
    {
        Serial.printf("%02X", idx);
    }
    Serial.print("\n\nNwsKey = ");
    for (unsigned char idx : g_lorawan_settings.node_nws_key)
    {
        Serial.printf("%02X", idx);
    }
    Serial.print("\n\nAppsKey = ");
    for (unsigned char idx : g_lorawan_settings.node_apps_key)
    {
        Serial.printf("%02X", idx);
    }
    Serial.printf("\n\nDevice Addr = %08lX\n", g_lorawan_settings.node_dev_addr);
#endif
}

/**
 * @brief   Initialize and load LoRaWAN settings from the flash.
 *
 * @details This function checks if there's valid data on the flash, if not,
 *          it generates default settings based on the device's flash ID.
 *
 * @note    No input parameters.
 * @retval  None.
 *
 * @author  Khalifa Alremeithi <kalremei@purdue.edu>
 */
void init_flash()
{
    // Check if valid data is in the flash
    s_lorawan_settings flash_settings;

    // Log the flash and sector size
    APP_LOG("FLASH", "Flash size %lX - Flash sector size %lX", PICO_FLASH_SIZE_BYTES, FLASH_SECTOR_SIZE);

    // Log the address and size of the data we are trying to read from flash
    APP_LOG("FLASH", "Trying to read from Flash address %lX", XIP_BASE + FLASH_TARGET_OFFSET);
    APP_LOG("FLASH", "Data size %ld", sizeof(s_lorawan_settings));

    // Disable interrupts while reading from flash
    uint32_t ints = save_and_disable_interrupts();
    memcpy((void *)&flash_settings, (void *)(XIP_BASE + FLASH_TARGET_OFFSET), sizeof(flash_settings));
    restore_interrupts(ints);

    // Log the marker and region values read from flash
    APP_LOG("FLASH", "Mark1: %0X Mark2: %0X", flash_settings.valid_mark_1, flash_settings.valid_mark_2);
    APP_LOG("FLASH", "Region: %d", flash_settings.lora_region);

    // Check if the marker values indicate valid data
    if ((flash_settings.valid_mark_1 != 0xAA) || (flash_settings.valid_mark_2 != LORAWAN_DATA_MARKER))
    {
        APP_LOG("FLASH", "No valid data found");

        // Create default settings from devices flash ID
        make_credentials();

        // Write new data to the flash
        eraseDataFlash();
        writeDataToFlash((uint8_t *)&g_lorawan_settings);
    }
    else
    {
        APP_LOG("FLASH", "Found valid data in flash");
        // Copy the data from flash to the global settings variable
        memcpy((void *)&g_lorawan_settings, (void *)&flash_settings, sizeof(flash_settings));
    }
}

/**
 * @brief   Save the current LoRaWAN settings to flash.
 *
 * @details This function compares the current settings with what's
 *          on the flash and updates the flash if needed.
 *
 * @note    No input parameters.
 * @retval  bool   Returns true after saving the settings.
 *
 * @author  Khalifa Alremeithi <kalremei@purdue.edu>
 */
bool save_settings()
{
    // Get settings from flash
    s_lorawan_settings flash_settings;

    // Disable interrupts to ensure data consistency during flash operation
    uint32_t ints = save_and_disable_interrupts();
    memcpy((void *)&flash_settings, (void *)(XIP_BASE + FLASH_TARGET_OFFSET), sizeof(flash_settings));
    restore_interrupts(ints);

    // Compare flash content with current settings
    if (memcmp((void *)&flash_settings, (void *)&g_lorawan_settings, sizeof(s_lorawan_settings)) != 0)
    {
        APP_LOG("FLASH", "Flash content changed, writing new data");
        // Erase flash before writing new data
        eraseDataFlash();
        // Write new data to the flash
        writeDataToFlash((uint8_t *)&g_lorawan_settings);
    }
    else
    {
        APP_LOG("FLASH", "Flash content identical no need to write");
    }
    return true;
}

/**
 * @brief   Reset the flash content with default LoRaWAN settings.
 *
 * @details This function creates a new set of default settings
 *          based on the device's flash ID and writes them to the flash.
 *
 * @note    No input parameters.
 * @retval  None.
 *
 * @author  Khalifa Alremeithi <kalremei@purdue.edu>
 */
void flash_reset()
{
    // Get a fresh structure
    s_lorawan_settings new_settings;
    memcpy((void *)&g_lorawan_settings, (void *)&new_settings, sizeof(new_settings));
    // Create default settings from devices flash ID
    make_credentials();

    // Erase the flash, then call init_flash to restore defaults
    eraseDataFlash();
    writeDataToFlash((uint8_t *)&g_lorawan_settings);
}

/**
 * @brief   Log the saved LoRaWAN settings.
 *
 * @details This function logs various LoRaWAN settings such as
 *          DevEUI, AppEUI, AppKey, NwsKey, AppsKey, and other settings.
 *
 * @note    No input parameters.
 * @retval  None.
 *
 * @author  Khalifa Alremeithi <kalremei@purdue.edu>
 */
void log_settings()
{
    APP_LOG("FLASH", "Saved settings:");
    APP_LOG("FLASH", "000 Marks: %02X %02X", g_lorawan_settings.valid_mark_1, g_lorawan_settings.valid_mark_2);
    APP_LOG("FLASH", "002 Dev EUI %02X%02X%02X%02X%02X%02X%02X%02X", g_lorawan_settings.node_device_eui[0],
            g_lorawan_settings.node_device_eui[1],
            g_lorawan_settings.node_device_eui[2], g_lorawan_settings.node_device_eui[3],
            g_lorawan_settings.node_device_eui[4], g_lorawan_settings.node_device_eui[5],
            g_lorawan_settings.node_device_eui[6], g_lorawan_settings.node_device_eui[7]);
    APP_LOG("FLASH", "010 App EUI %02X%02X%02X%02X%02X%02X%02X%02X", g_lorawan_settings.node_app_eui[0],
            g_lorawan_settings.node_app_eui[1],
            g_lorawan_settings.node_app_eui[2], g_lorawan_settings.node_app_eui[3],
            g_lorawan_settings.node_app_eui[4], g_lorawan_settings.node_app_eui[5],
            g_lorawan_settings.node_app_eui[6], g_lorawan_settings.node_app_eui[7]);
    APP_LOG("FLASH", "018 App Key %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            g_lorawan_settings.node_app_key[0], g_lorawan_settings.node_app_key[1],
            g_lorawan_settings.node_app_key[2], g_lorawan_settings.node_app_key[3],
            g_lorawan_settings.node_app_key[4], g_lorawan_settings.node_app_key[5],
            g_lorawan_settings.node_app_key[6], g_lorawan_settings.node_app_key[7],
            g_lorawan_settings.node_app_key[8], g_lorawan_settings.node_app_key[9],
            g_lorawan_settings.node_app_key[10], g_lorawan_settings.node_app_key[11],
            g_lorawan_settings.node_app_key[12], g_lorawan_settings.node_app_key[13],
            g_lorawan_settings.node_app_key[14], g_lorawan_settings.node_app_key[15]);
    APP_LOG("FLASH", "034 Dev Addr %08lX", g_lorawan_settings.node_dev_addr);
    APP_LOG("FLASH", "038 NWS Key %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            g_lorawan_settings.node_nws_key[0], g_lorawan_settings.node_nws_key[1],
            g_lorawan_settings.node_nws_key[2], g_lorawan_settings.node_nws_key[3],
            g_lorawan_settings.node_nws_key[4], g_lorawan_settings.node_nws_key[5],
            g_lorawan_settings.node_nws_key[6], g_lorawan_settings.node_nws_key[7],
            g_lorawan_settings.node_nws_key[8], g_lorawan_settings.node_nws_key[9],
            g_lorawan_settings.node_nws_key[10], g_lorawan_settings.node_nws_key[11],
            g_lorawan_settings.node_nws_key[12], g_lorawan_settings.node_nws_key[13],
            g_lorawan_settings.node_nws_key[14], g_lorawan_settings.node_nws_key[15]);
    APP_LOG("FLASH", "054 Apps Key %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            g_lorawan_settings.node_apps_key[0], g_lorawan_settings.node_apps_key[1],
            g_lorawan_settings.node_apps_key[2], g_lorawan_settings.node_apps_key[3],
            g_lorawan_settings.node_apps_key[4], g_lorawan_settings.node_apps_key[5],
            g_lorawan_settings.node_apps_key[6], g_lorawan_settings.node_apps_key[7],
            g_lorawan_settings.node_apps_key[8], g_lorawan_settings.node_apps_key[9],
            g_lorawan_settings.node_apps_key[10], g_lorawan_settings.node_apps_key[11],
            g_lorawan_settings.node_apps_key[12], g_lorawan_settings.node_apps_key[13],
            g_lorawan_settings.node_apps_key[14], g_lorawan_settings.node_apps_key[15]);
    APP_LOG("FLASH", "070 OTAA %s", g_lorawan_settings.otaa_enabled ? "enabled" : "disabled");
    APP_LOG("FLASH", "071 ADR %s", g_lorawan_settings.adr_enabled ? "enabled" : "disabled");
    APP_LOG("FLASH", "072 %s Network", g_lorawan_settings.public_network ? "Public" : "Private");
    APP_LOG("FLASH", "073 Dutycycle %s", g_lorawan_settings.duty_cycle_enabled ? "enabled" : "disabled");
    APP_LOG("FLASH", "074 Repeat time %ld", g_lorawan_settings.send_repeat_time);
    APP_LOG("FLASH", "078 Join trials %d", g_lorawan_settings.join_trials);
    APP_LOG("FLASH", "079 TX Power %d", g_lorawan_settings.tx_power);
    APP_LOG("FLASH", "080 DR %d", g_lorawan_settings.data_rate);
    APP_LOG("FLASH", "081 Class %d", g_lorawan_settings.lora_class);
    APP_LOG("FLASH", "082 Subband %d", g_lorawan_settings.subband_channels);
    APP_LOG("FLASH", "083 Auto join %s", g_lorawan_settings.auto_join ? "enabled" : "disabled");
    APP_LOG("FLASH", "084 Fport %d", g_lorawan_settings.app_port);
    APP_LOG("FLASH", "085 %s Message", g_lorawan_settings.confirmed_msg_enabled ? "Confirmed" : "Unconfirmed");
    APP_LOG("FLASH", "086 Region %s", region_names[g_lorawan_settings.lora_region]);
    APP_LOG("FLASH", "087 Mode %s", g_lorawan_settings.lorawan_enable ? "LPWAN" : "P2P");
}