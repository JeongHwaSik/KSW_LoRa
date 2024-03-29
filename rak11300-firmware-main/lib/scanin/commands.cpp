#include "scanin.h"

static char atcmd[ATCMD_SIZE];
static uint16_t atcmd_index = 0;
static char g_at_query_buf[ATQUERY_SIZE];

/**
 * @brief Converts a hexadecimal string to a binary array.
 *
 * @param hex Input hexadecimal string.
 * @param bin Output binary array.
 * @param bin_length Maximum length of the binary array.
 * @return Length of the converted binary data or -1 on error.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
int hex2bin(const char *hex, uint8_t *bin, uint16_t bin_length)
{
    uint16_t hex_length = strlen(hex);
    const char *hex_end = hex + hex_length;
    uint8_t *cur = bin;
    uint8_t num_chars = hex_length & 1;
    uint8_t byte = 0;

    if (hex_length % 2 != 0)
    {
        return -1;
    }

    if (hex_length / 2 > bin_length)
    {
        return -1;
    }

    while (hex < hex_end)
    {
        if ('A' <= *hex && *hex <= 'F')
        {
            byte |= 10 + (*hex - 'A');
        }
        else if ('a' <= *hex && *hex <= 'f')
        {
            byte |= 10 + (*hex - 'a');
        }
        else if ('0' <= *hex && *hex <= '9')
        {
            byte |= *hex - '0';
        }
        else
        {
            return -1;
        }
        hex++;
        num_chars++;

        if (num_chars >= 2)
        {
            num_chars = 0;
            *cur++ = byte;
            byte = 0;
        }
        else
        {
            byte <<= 4;
        }
    }
    return cur - bin;
}

/**
 * @brief Queries the software version.
 *
 * @return Always returns 0.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
int at_query_version()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d.%d.%d %s %s", g_sw_ver_1, g_sw_ver_2, g_sw_ver_3, __DATE__, __TIME__);

    return 0;
}

/**
 * @brief Displays the device settings.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void at_settings()
{
    AT_PRINTF("Device status:\n")
    AT_PRINTF("   Auto join %s\n", g_lorawan_settings.auto_join ? "enabled" : "disabled")
    AT_PRINTF("   Mode %s\n", g_lorawan_settings.lorawan_enable ? "LPWAN" : "P2P")
    AT_PRINTF("LPWAN status:\n")
    AT_PRINTF("   Marks: %02X %02X\n", g_lorawan_settings.valid_mark_1, g_lorawan_settings.valid_mark_2)
    AT_PRINTF("   Dev EUI %02X%02X%02X%02X%02X%02X%02X%02X\n", g_lorawan_settings.node_device_eui[0],
              g_lorawan_settings.node_device_eui[1],
              g_lorawan_settings.node_device_eui[2], g_lorawan_settings.node_device_eui[3],
              g_lorawan_settings.node_device_eui[4], g_lorawan_settings.node_device_eui[5],
              g_lorawan_settings.node_device_eui[6], g_lorawan_settings.node_device_eui[7])
    AT_PRINTF("   App EUI %02X%02X%02X%02X%02X%02X%02X%02X\n", g_lorawan_settings.node_app_eui[0],
              g_lorawan_settings.node_app_eui[1],
              g_lorawan_settings.node_app_eui[2], g_lorawan_settings.node_app_eui[3],
              g_lorawan_settings.node_app_eui[4], g_lorawan_settings.node_app_eui[5],
              g_lorawan_settings.node_app_eui[6], g_lorawan_settings.node_app_eui[7])
    AT_PRINTF("   App Key %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
              g_lorawan_settings.node_app_key[0], g_lorawan_settings.node_app_key[1],
              g_lorawan_settings.node_app_key[2], g_lorawan_settings.node_app_key[3],
              g_lorawan_settings.node_app_key[4], g_lorawan_settings.node_app_key[5],
              g_lorawan_settings.node_app_key[6], g_lorawan_settings.node_app_key[7],
              g_lorawan_settings.node_app_key[8], g_lorawan_settings.node_app_key[9],
              g_lorawan_settings.node_app_key[10], g_lorawan_settings.node_app_key[11],
              g_lorawan_settings.node_app_key[12], g_lorawan_settings.node_app_key[13],
              g_lorawan_settings.node_app_key[14], g_lorawan_settings.node_app_key[15])
    AT_PRINTF("   Dev Addr %08lX\n", g_lorawan_settings.node_dev_addr)
    AT_PRINTF("   NWS Key %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
              g_lorawan_settings.node_nws_key[0], g_lorawan_settings.node_nws_key[1],
              g_lorawan_settings.node_nws_key[2], g_lorawan_settings.node_nws_key[3],
              g_lorawan_settings.node_nws_key[4], g_lorawan_settings.node_nws_key[5],
              g_lorawan_settings.node_nws_key[6], g_lorawan_settings.node_nws_key[7],
              g_lorawan_settings.node_nws_key[8], g_lorawan_settings.node_nws_key[9],
              g_lorawan_settings.node_nws_key[10], g_lorawan_settings.node_nws_key[11],
              g_lorawan_settings.node_nws_key[12], g_lorawan_settings.node_nws_key[13],
              g_lorawan_settings.node_nws_key[14], g_lorawan_settings.node_nws_key[15])
    AT_PRINTF("   Apps Key %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
              g_lorawan_settings.node_apps_key[0], g_lorawan_settings.node_apps_key[1],
              g_lorawan_settings.node_apps_key[2], g_lorawan_settings.node_apps_key[3],
              g_lorawan_settings.node_apps_key[4], g_lorawan_settings.node_apps_key[5],
              g_lorawan_settings.node_apps_key[6], g_lorawan_settings.node_apps_key[7],
              g_lorawan_settings.node_apps_key[8], g_lorawan_settings.node_apps_key[9],
              g_lorawan_settings.node_apps_key[10], g_lorawan_settings.node_apps_key[11],
              g_lorawan_settings.node_apps_key[12], g_lorawan_settings.node_apps_key[13],
              g_lorawan_settings.node_apps_key[14], g_lorawan_settings.node_apps_key[15])
    AT_PRINTF("   OTAA %s\n", g_lorawan_settings.otaa_enabled ? "enabled" : "disabled")
    AT_PRINTF("   ADR %s\n", g_lorawan_settings.adr_enabled ? "enabled" : "disabled")
    AT_PRINTF("   %s Network\n", g_lorawan_settings.public_network ? "Public" : "Private")
    AT_PRINTF("   Dutycycle %s\n", g_lorawan_settings.duty_cycle_enabled ? "enabled" : "disabled")
    AT_PRINTF("   Repeat time %ld\n", g_lorawan_settings.send_repeat_time)
    AT_PRINTF("   Join trials %d\n", g_lorawan_settings.join_trials)
    AT_PRINTF("   TX Power %d\n", g_lorawan_settings.tx_power)
    AT_PRINTF("   DR %d\n", g_lorawan_settings.data_rate)
    AT_PRINTF("   Class %d\n", g_lorawan_settings.lora_class)
    AT_PRINTF("   Subband %d\n", g_lorawan_settings.subband_channels)
    AT_PRINTF("   Fport %d\n", g_lorawan_settings.app_port)
    AT_PRINTF("   %s Message\n", g_lorawan_settings.confirmed_msg_enabled ? "Confirmed" : "Unconfirmed")
    AT_PRINTF("   Region %s\n", region_names[g_lorawan_settings.lora_region])
    AT_PRINTF("   Network %s\n", g_lpwan_has_joined ? "joined" : "not joined")
}

/**
 * @brief Queries the mode of operation.
 *
 * @return Always returns 0.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
int at_query_mode()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", g_lorawan_settings.lorawan_enable ? 1 : 0);
    return 0;
}

/**
 * @brief Executes the AT command for enabling or disabling the LoRaWAN feature.
 *
 * @param str Pointer to the input string. '0' to disable, '1' to enable.
 *
 * @return 0 for success, error code otherwise.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
int at_exec_mode(char *str)
{
    bool need_restart = false;
    if (str[0] == '0')
    {
        if (g_lorawan_settings.lorawan_enable)
        {
            need_restart = true;
        }
        g_lorawan_settings.lorawan_enable = false;
    }
    else if (str[0] == '1')
    {
        if (!g_lorawan_settings.lorawan_enable)
        {
            need_restart = true;
        }
        g_lorawan_settings.lorawan_enable = true;
    }
    else
    {
        return AT_ERRNO_PARA_VAL;
    }

    save_settings();

    if (need_restart)
    {
        delay(100);
        NVIC_SystemReset();
    }
    return 0;
}

/**
 * @brief Queries the current LoRaWAN region.
 * @return 0 for success, error code otherwise.
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
int at_query_region()
{
    // 0: AS923 1: AU915 2: CN470 3: CN779 4: EU433 5: EU868 6: KR920 7: IN865 8: US915 9: AS923-2 10: AS923-3 11: AS923-4 12: RU864
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", g_lorawan_settings.lora_region);

    return 0;
}

/**
 * @brief Executes the AT command for setting the LoRaWAN region.
 * @param str Pointer to the input string indicating the desired region.
 * @return 0 for success, error code otherwise.
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
int at_exec_region(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    char *param;
    uint8_t region;

    param = strtok(str, ",");
    if (param != nullptr)
    {
        region = strtol(param, nullptr, 0);
        // RAK4630 0: AS923 1: AU915 2: CN470 3: CN779 4: EU433 5: EU868 6: KR920 7: IN865 8: US915 9: AS923-2 10: AS923-3 11: AS923-4 12: RU864
        if (region > 12)
        {
            return AT_ERRNO_PARA_VAL;
        }
        g_lorawan_settings.lora_region = region;
        save_settings();
    }
    else
    {
        return AT_ERRNO_PARA_VAL;
    }

    return 0;
}

int at_query_mask()
{
    if ((g_lorawan_settings.lora_region == 1) || (g_lorawan_settings.lora_region == 2) ||
        (g_lorawan_settings.lora_region == 8))
    {
        snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", g_lorawan_settings.subband_channels);

        return 0;
    }
    return AT_ERRNO_PARA_VAL;
}

int at_exec_mask(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    char *param;
    uint8_t mask;

    param = strtok(str, ",");
    if (param != nullptr)
    {
        mask = strtol(param, nullptr, 0);

        uint8_t maxBand;
        switch (g_lorawan_settings.lora_region)
        {
        case LORAMAC_REGION_AU915:
            maxBand = 9;
            break;
        case LORAMAC_REGION_CN470:
            maxBand = 12;
            break;
        case LORAMAC_REGION_US915:
            maxBand = 9;
            break;
        default:
            return AT_ERRNO_PARA_VAL;
        }
        if ((mask == 0) || (mask > maxBand))
        {
            return AT_ERRNO_PARA_VAL;
        }
        g_lorawan_settings.subband_channels = mask;
        save_settings();
    }
    else
    {
        return AT_ERRNO_PARA_NUM;
    }

    return 0;
}

int at_query_joinmode()
{
    int mode;
    mode = g_lorawan_settings.otaa_enabled ? 1 : 0;

    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", mode);
    return 0;
}

int at_exec_joinmode(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    int mode = strtol(str, nullptr, 0);

    if (mode != 0 && mode != 1)
    {
        return AT_ERRNO_PARA_VAL;
    }

    g_lorawan_settings.otaa_enabled = mode == 1;
    save_settings();

    return 0;
}

int at_query_deveui()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE,
             "%02X%02X%02X%02X%02X%02X%02X%02X",
             g_lorawan_settings.node_device_eui[0],
             g_lorawan_settings.node_device_eui[1],
             g_lorawan_settings.node_device_eui[2],
             g_lorawan_settings.node_device_eui[3],
             g_lorawan_settings.node_device_eui[4],
             g_lorawan_settings.node_device_eui[5],
             g_lorawan_settings.node_device_eui[6],
             g_lorawan_settings.node_device_eui[7]);
    return 0;
}

int at_exec_deveui(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    uint8_t len;
    uint8_t buf[8];

    len = hex2bin(str, buf, 8);

    if (len != 8)
    {
        return AT_ERRNO_PARA_VAL;
    }

    memcpy(g_lorawan_settings.node_device_eui, buf, 8);
    save_settings();

    return 0;
}

int at_query_appeui()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE,
             "%02X%02X%02X%02X%02X%02X%02X%02X",
             g_lorawan_settings.node_app_eui[0],
             g_lorawan_settings.node_app_eui[1],
             g_lorawan_settings.node_app_eui[2],
             g_lorawan_settings.node_app_eui[3],
             g_lorawan_settings.node_app_eui[4],
             g_lorawan_settings.node_app_eui[5],
             g_lorawan_settings.node_app_eui[6],
             g_lorawan_settings.node_app_eui[7]);
    return 0;
}

int at_exec_appeui(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    uint8_t len;
    uint8_t buf[8];

    len = hex2bin(str, buf, 8);
    if (len != 8)
    {
        return AT_ERRNO_PARA_VAL;
    }

    memcpy(g_lorawan_settings.node_app_eui, buf, 8);
    save_settings();

    return 0;
}

int at_query_appkey()
{
    uint8_t i;
    uint8_t len = 0;

    for (i = 0; i < 16; i++)
    {
        len += snprintf(g_at_query_buf + len, ATQUERY_SIZE - len, "%02X", g_lorawan_settings.node_app_key[i]);
        if (ATQUERY_SIZE <= len)
        {
            return -1;
        }
    }
    return 0;
}

int at_exec_appkey(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    uint8_t buf[16];
    uint8_t len;

    len = hex2bin(str, buf, 16);
    if (len != 16)
    {
        return AT_ERRNO_PARA_VAL;
    }

    memcpy(g_lorawan_settings.node_app_key, buf, 16);
    save_settings();

    return 0;
}

int at_query_devaddr()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%08lX\n", g_lorawan_settings.node_dev_addr);
    return 0;
}

int at_exec_devaddr(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    int i;
    uint8_t len;
    uint8_t buf[4];
    uint8_t swap_buf[4];

    len = hex2bin(str, buf, 4);
    if (len != 4)
    {
        return AT_ERRNO_PARA_VAL;
    }

    for (i = 0; i < 4; i++)
    {
        swap_buf[i] = buf[3 - i];
    }

    memcpy(&g_lorawan_settings.node_dev_addr, swap_buf, 4);
    save_settings();

    return 0;
}

int at_query_appskey()
{
    uint8_t i;
    uint8_t len = 0;

    for (i = 0; i < 16; i++)
    {
        len += snprintf(g_at_query_buf + len, ATQUERY_SIZE - len, "%02X", g_lorawan_settings.node_apps_key[i]);
        if (ATQUERY_SIZE <= len)
        {
            return -1;
        }
    }

    return 0;
}

int at_exec_appskey(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    uint8_t len;
    uint8_t buf[16];

    len = hex2bin(str, buf, 16);
    if (len != 16)
    {
        return AT_ERRNO_PARA_VAL;
    }

    memcpy(g_lorawan_settings.node_apps_key, buf, 16);
    save_settings();

    return 0;
}

int at_query_nwkskey()
{
    uint8_t i;
    uint8_t len = 0;

    for (i = 0; i < 16; i++)
    {
        len += snprintf(g_at_query_buf + len, ATQUERY_SIZE - len, "%02X", g_lorawan_settings.node_nws_key[i]);
        if (ATQUERY_SIZE <= len)
        {
            return -1;
        }
    }

    return 0;
}

int at_exec_nwkskey(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    uint8_t len;
    uint8_t buf[16];

    len = hex2bin(str, buf, 16);
    if (len != 16)
    {
        return AT_ERRNO_PARA_VAL;
    }

    memcpy(g_lorawan_settings.node_nws_key, buf, 16);
    save_settings();

    return 0;
}

int at_query_class()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%c", g_lorawan_settings.lora_class + 65);

    return 0;
}

int at_exec_class(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    uint8_t cls;
    char *param;

    param = strtok(str, ",");
    cls = (uint8_t)param[0];
    // Class B is not supported
    // if (cls != 'A' && cls != 'B' && cls != 'C')
    if (cls != 'A' && cls != 'C')
    {
        return AT_ERRNO_PARA_VAL;
    }

    g_lorawan_settings.lora_class = cls - 65;
    save_settings();

    return 0;
}

int at_query_join()
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    // Param1 = Join command: 1 for joining the network , 0 for stop joining
    // Param2 = Auto-Join config: 1 for Auto-join on power up) , 0 for no auto-join.
    // Param3 = Reattempt interval: 7 - 255 seconds
    // Param4 = No. of join attempts: 0 - 255
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d,%d,%d,%d", 0, g_lorawan_settings.auto_join, 8,
             g_lorawan_settings.join_trials);

    return 0;
}

int at_exec_join(char *str)
{
    uint8_t bJoin;
    uint8_t autoJoin;
    uint8_t nbtrials;
    char *param;

    param = strtok(str, ":");

    /* check start or stop join parameter */
    bJoin = strtol(param, nullptr, 0);
    if (bJoin != 1 && bJoin != 0)
    {
        return AT_ERRNO_PARA_VAL;
    }

    /* check auto join parameter */
    param = strtok(nullptr, ":");
    if (param != nullptr)
    {
        autoJoin = strtol(param, nullptr, 0);
        if (autoJoin != 1 && autoJoin != 0)
        {
            return AT_ERRNO_PARA_VAL;
        }
        g_lorawan_settings.auto_join = autoJoin == 1;

        param = strtok(nullptr, ":");
        if (param != nullptr)
        {
            // join interval, not support yet.

            // join attemps number
            param = strtok(nullptr, ":");
            if (param != nullptr)
            {
                nbtrials = strtol(param, nullptr, 0);
                if ((nbtrials == 0) && g_lorawan_settings.lorawan_enable)
                {
                    return AT_ERRNO_PARA_VAL;
                }
                g_lorawan_settings.join_trials = nbtrials;
            }
        }
        save_settings();

        if ((bJoin == 1) && !g_lorawan_initialized) // ==0 stop join, not support, yet
        {
            APP_LOG("AT", "Initialize LoRaWAN and start join");
            // Manual join only works if LoRaWAN was not initialized yet.
            // If LoRaWAN was already initialized, a restart is required
            // Wake up task to request join
            if (loop_thread != nullptr)
            {
                APP_LOG("AT", "Request Join");
                osSignalSet(loop_thread, SIGNAL_JOIN);
            }
        }

        if ((bJoin == 1) && g_lorawan_initialized && (lmh_join_status_get() != LMH_SET))
        {
            // If if not yet joined, start join
            APP_LOG("AT", "Start Join");
            delay(100);
            lmh_join();
            return 0;
        }

        // if ((bJoin == 1) && g_lorawan_settings.auto_join)
        // {
        // 	// If auto join is set, restart the device to start join process
        // 	delay(100);
        // 	NVIC_SystemReset();
        // 	return 0;
        // }
    }
    else
    {
        return AT_ERRNO_PARA_VAL;
    }

    return 0;
}

int at_query_join_status()
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    uint8_t join_status;

    join_status = (uint8_t)lmh_join_status_get();
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", join_status);

    return 0;
}

int at_query_confirm()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", g_lorawan_settings.confirmed_msg_enabled);
    return 0;
}

int at_exec_confirm(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    int cfm;

    cfm = strtol(str, nullptr, 0);
    if (cfm != 0 && cfm != 1)
    {
        return AT_ERRNO_PARA_VAL;
    }

    g_lorawan_settings.confirmed_msg_enabled = (lmh_confirm)cfm;
    save_settings();

    return 0;
}

int at_query_datarate()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", g_lorawan_settings.data_rate);
    return 0;
}

int at_exec_datarate(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    uint8_t datarate;

    datarate = strtol(str, nullptr, 0);

    if (datarate > 15)
    {
        return AT_ERRNO_PARA_VAL;
    }

    g_lorawan_settings.data_rate = datarate;
    save_settings();

    return 0;
}

int at_query_adr()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", g_lorawan_settings.adr_enabled ? 1 : 0);
    return 0;
}

int at_exec_adr(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    int adr;

    adr = strtol(str, nullptr, 0);
    if (adr != 0 && adr != 1)
    {
        return AT_ERRNO_PARA_VAL;
    }

    g_lorawan_settings.adr_enabled = adr == 1;

    save_settings();

    return 0;
}

int at_query_txpower()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", g_lorawan_settings.tx_power);
    return 0;
}

int at_exec_txpower(char *str)
{
    if (!g_lorawan_settings.lorawan_enable)
    {
        return AT_ERRNO_NOALLOW;
    }
    uint8_t tx_power;

    tx_power = strtol(str, nullptr, 0);

    if (tx_power > 10)
    {
        return AT_ERRNO_PARA_VAL;
    }

    g_lorawan_settings.tx_power = tx_power;

    save_settings();

    return 0;
}

int at_query_sendfreq()
{
    // Return time in seconds, but it is saved in milli seconds
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d",
             (g_lorawan_settings.send_repeat_time == 0) ? 0 : (int)(g_lorawan_settings.send_repeat_time / 1000));

    return 0;
}

int at_exec_sendfreq(char *str)
{
    long time = strtol(str, nullptr, 0);

    if ((time < 0) || (time > 3600))
    {
        return AT_ERRNO_PARA_VAL;
    }

    g_lorawan_settings.send_repeat_time = time * 1000;

    if (g_lorawan_settings.send_repeat_time != 0)
    {
        TimerStop(&app_timer);
        TimerSetValue(&app_timer, g_lorawan_settings.send_repeat_time);
        TimerStart(&app_timer);
    }
    else
    {
        TimerStop(&app_timer);
    }

    save_settings();

    return 0;
}

int at_exec_send(char *str)
{
    if (!g_lpwan_has_joined || !g_lorawan_settings.lorawan_enable)
    {
        Serial.printf("joined = %s\n", g_lpwan_has_joined ? "true" : "false");
        Serial.printf("enabled = %s\n", g_lorawan_settings.lorawan_enable ? "true" : "false");
        return AT_ERRNO_NOALLOW;
    }

    // Get fPort
    char *param;

    param = strtok(str, ":");
    uint16_t fPort = strtol(param, nullptr, 0);
    if ((fPort == 0) || (fPort > 255))
    {
        return AT_ERRNO_PARA_VAL;
    }

    // Get data to send
    param = strtok(nullptr, ":");
    int data_size = strlen(param);
    if (data_size % 2 != 0 || (data_size > 254))
    {
        return AT_ERRNO_PARA_VAL;
    }

    int buff_idx = 0;
    char buff_parse[3];
    for (int idx = 0; idx <= data_size + 1; idx += 2)
    {
        buff_parse[0] = param[idx];
        buff_parse[1] = param[idx + 1];
        buff_parse[2] = 0;
        m_lora_app_data_buffer[buff_idx] = strtol(buff_parse, nullptr, 16);
        buff_idx++;
    }
    send_lora_packet(m_lora_app_data_buffer, data_size / 2, fPort);
    return 0;
}

int at_query_battery()
{
    // Battery level is from 1 to 254, 254 meaning fully charged.
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", get_lora_batt());

    return 0;
}

int at_query_rssi()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", g_last_rssi);

    return 0;
}

int at_query_snr()
{
    snprintf(g_at_query_buf, ATQUERY_SIZE, "%d", g_last_snr);

    return 0;
}

/**
 * @brief Executes the AT command for rebooting the system.
 * @return Does not return as the system reboots.
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
int at_exec_reboot()
{
    delay(100);
    NVIC_SystemReset();
}

int at_query_status()
{
    at_settings();
    snprintf(g_at_query_buf, ATQUERY_SIZE, " ");

    return 0;
}

/**
 * @brief Executes the AT command to restore default settings.
 * @return 0 for success, error code otherwise.
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
int at_exec_restore()
{
    flash_reset();
    return 0;
}

atcmd_t g_at_cmd_list[30] = {
    /*|    CMD    |     AT+CMD?      |    AT+CMD=?    |  AT+CMD=value |  AT+CMD  |*/
    // General commands
    {"?", "AT commands", nullptr, nullptr, at_exec_list_all},
    {"R", "Restore default", nullptr, nullptr, at_exec_restore},
    {"Z", "Trig a MCU reset", nullptr, nullptr, at_exec_reboot},
    // LoRaWAN keys, ID's EUI's
    {"+APPEUI", "Get or set the application EUI", at_query_appeui, at_exec_appeui, nullptr},
    {"+APPKEY", "Get or set the application key", at_query_appkey, at_exec_appkey, nullptr},
    {"+DEVEUI", "Get or set the device EUI", at_query_deveui, at_exec_deveui, nullptr},
    {"+APPSKEY", "Get or set the application session key", at_query_appskey, at_exec_appskey, nullptr},
    {"+NWKSKEY", "Get or Set the network session key", at_query_nwkskey, at_exec_nwkskey, nullptr},
    {"+DEVADDR", "Get or set the device address", at_query_devaddr, at_exec_devaddr, nullptr},
    // Joining and sending data on LoRa network
    {"+CFM", "Get or set the confirm mode", at_query_confirm, at_exec_confirm, nullptr},
    {"+JOIN", "Join network", at_query_join, at_exec_join, nullptr},
    {"+NJS", "Get the join status", at_query_join_status, nullptr, nullptr},
    {"+NJM", "Get or set the network join mode", at_query_joinmode, at_exec_joinmode, nullptr},
    {"+SENDFREQ", "Get or Set the automatic send time", at_query_sendfreq, at_exec_sendfreq, nullptr},
    {"+SEND", "Send data", nullptr, at_exec_send, nullptr},
    // LoRa network management
    {"+ADR", "Get or set the adaptive data rate setting", at_query_adr, at_exec_adr, nullptr},
    {"+CLASS", "Get or set the device class", at_query_class, at_exec_class, nullptr},
    {"+DR", "Get or Set the Tx DataRate=[0..7]", at_query_datarate, at_exec_datarate, nullptr},
    {"+TXP", "Get or set the transmit power", at_query_txpower, at_exec_txpower, nullptr},
    {"+BAND", "Get and Set number corresponding to active regions", at_query_region, at_exec_region, nullptr},
    {"+MASK", "Get and Set channels mask", at_query_mask, at_exec_mask, nullptr},
    // Status queries
    {"+BAT", "Get battery level", at_query_battery, nullptr, nullptr},
    {"+RSSI", "Last RX packet RSSI", at_query_rssi, nullptr, nullptr},
    {"+SNR", "Last RX packet SNR", at_query_snr, nullptr, nullptr},
    {"+STATUS", "Show LoRaWAN status", at_query_status, nullptr, nullptr}};

/**
 * @brief Lists all available AT commands.
 * @return 0 for success, error code otherwise.
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
int at_exec_list_all()
{
    AT_PRINTF("\r\n+++++++++++++++\r\n");
    AT_PRINTF("AT command list\r\n");
    AT_PRINTF("+++++++++++++++\r\n");

    for (unsigned int idx = 0; idx < (sizeof(g_at_cmd_list)) / sizeof(atcmd_t); idx++)
    {
        if (idx < 4)
        {
            AT_PRINTF("AT%s\t\t%s\r\n", g_at_cmd_list[idx].cmd_name, g_at_cmd_list[idx].cmd_desc);
        }
        else
        {
            AT_PRINTF("AT%s\t%s\r\n", g_at_cmd_list[idx].cmd_name, g_at_cmd_list[idx].cmd_desc);
        }
    }
    AT_PRINTF("+++++++++++++++\r\n");
    return 0;
}

/**
 * @brief Handles the execution of AT commands based on user input.
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void at_cmd_handle()
{
    uint8_t i;
    int ret = 0;
    const char *cmd_name;
    char *rxcmd = atcmd + 2;
    int16_t tmp = atcmd_index - 2;
    uint16_t rxcmd_index;

    if (atcmd_index < 2 || rxcmd[tmp] != '\0')
    {
        atcmd_index = 0;
        memset(atcmd, 0xff, ATCMD_SIZE);
        return;
    }

    // Serial.printf("atcmd_index==%d=%s==\n", atcmd_index, atcmd);
    if (atcmd_index == 2 && strncmp(atcmd, "AT", atcmd_index) == 0)
    {
        atcmd_index = 0;
        memset(atcmd, 0xff, ATCMD_SIZE);
        AT_PRINTF("\r\nOK\r\n");
        return;
    }

    rxcmd_index = tmp;

    for (i = 0; i < sizeof(g_at_cmd_list) / sizeof(atcmd_t); i++)
    {
        cmd_name = g_at_cmd_list[i].cmd_name;
        // Serial.printf("===rxcmd========%s================cmd_name=====%s====%d===\n", rxcmd, cmd_name, strlen(cmd_name));
        if (strlen(cmd_name) && strncmp(rxcmd, cmd_name, strlen(cmd_name)) != 0)
        {
            continue;
        }

        // Serial.printf("===rxcmd_index========%d================strlen(cmd_name)=====%d=======\n", rxcmd_index, strlen(cmd_name));

        if (rxcmd_index == (strlen(cmd_name) + 1) &&
            rxcmd[strlen(cmd_name)] == '?')
        {
            /* test cmd */
            if (g_at_cmd_list[i].cmd_desc)
            {
                if (strncmp(g_at_cmd_list[i].cmd_desc, "OK", 2) == 0)
                {
                    snprintf(atcmd, ATCMD_SIZE, "\r\nOK\r\n");
                }
                else
                {
                    snprintf(atcmd, ATCMD_SIZE, "\r\n%s:\"%s\"\r\nOK\r\n",
                             cmd_name, g_at_cmd_list[i].cmd_desc);
                }
            }
            else
            {
                snprintf(atcmd, ATCMD_SIZE, "\r\n%s\r\nOK\r\n", cmd_name);
            }
        }
        else if (rxcmd_index == (strlen(cmd_name) + 2) &&
                 strcmp(&rxcmd[strlen(cmd_name)], "=?") == 0)
        {
            /* query cmd */
            if (g_at_cmd_list[i].query_cmd != nullptr)
            {
                ret = g_at_cmd_list[i].query_cmd();

                if (ret == 0)
                {
                    snprintf(atcmd, ATCMD_SIZE, "\r\n%s:%s\r\nOK\r\n",
                             cmd_name, g_at_query_buf);
                }
            }
            else
            {
                ret = AT_ERRNO_NOALLOW;
            }
        }
        else if (rxcmd_index > (strlen(cmd_name) + 1) &&
                 rxcmd[strlen(cmd_name)] == '=')
        {
            /* exec cmd */
            if (g_at_cmd_list[i].exec_cmd != nullptr)
            {
                ret = g_at_cmd_list[i].exec_cmd(rxcmd + strlen(cmd_name) + 1);
                if (ret == 0)
                {
                    snprintf(atcmd, ATCMD_SIZE, "\r\nOK\r\n");
                }
                else if (ret == -1)
                {
                    ret = AT_ERRNO_SYS;
                }
            }
            else
            {
                ret = AT_ERRNO_NOALLOW;
            }
        }
        else if (rxcmd_index == strlen(cmd_name))
        {
            /* exec cmd without parameter*/
            if (g_at_cmd_list[i].exec_cmd_no_para != nullptr)
            {
                ret = g_at_cmd_list[i].exec_cmd_no_para();
                if (ret == 0)
                {
                    snprintf(atcmd, ATCMD_SIZE, "\r\nOK\r\n");
                }
                else if (ret == -1)
                {
                    ret = AT_ERRNO_SYS;
                }
            }
            else
            {
                ret = AT_ERRNO_NOALLOW;
            }
        }
        else
        {
            continue;
        }
        break;
    }

    // Check if user defined AT commands are setup
    if (i == sizeof(g_at_cmd_list) / sizeof(atcmd_t))
    {
        if (user_at_handler != nullptr)
        {
            if (user_at_handler(rxcmd, rxcmd_index))
            {
                ret = 0;
                snprintf(atcmd, ATCMD_SIZE, "\r\nOK\r\n");
            }
            else
            {
                ret = AT_ERRNO_NOSUPP;
            }
        }
        else
        {
            ret = AT_ERRNO_NOSUPP;
        }
    }

    if (ret != 0 && ret != AT_CB_PRINT)
    {
        snprintf(atcmd, ATCMD_SIZE, "\r\n%s%x\r\n", AT_ERROR, ret);
    }

    if (ret != AT_CB_PRINT)
    {
        AT_PRINTF(atcmd);
    }

    atcmd_index = 0;
    memset(atcmd, 0xff, ATCMD_SIZE);
}

/**
 * @brief Processes serial input for AT commands.
 * @param cmd The character received from serial input.
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void at_serial_input(uint8_t cmd)
{
    DualSerial("%c", cmd);

    // Handle backspace
    if (cmd == '\b')
    {
        atcmd[atcmd_index--] = '\0';
        DualSerial(" \b");
    }

    if ((cmd >= '0' && cmd <= '9') || (cmd >= 'a' && cmd <= 'z') ||
        (cmd >= 'A' && cmd <= 'Z') || cmd == '?' || cmd == '+' || cmd == ':' ||
        cmd == '=' || cmd == ' ' || cmd == ',')
    {
        atcmd[atcmd_index++] = cmd;
    }
    else if (cmd == '\r' || cmd == '\n')
    {
        atcmd[atcmd_index] = '\0';
        at_cmd_handle();
    }

    if (atcmd_index >= ATCMD_SIZE)
    {
        atcmd_index = 0;
    }
}