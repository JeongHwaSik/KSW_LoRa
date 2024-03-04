#ifndef SCAN_RAK_COMMANDS_H
#define SCAN_RAK_COMMANDS_H

#define AT_PRINTF(...) \
    Serial.printf(__VA_ARGS__);

// DEFINE LOCAL CONSTANTS
#define AT_ERROR "+CME ERROR:"
#define ATCMD_SIZE 160
#define ATQUERY_SIZE 128
#define AT_ERRNO_NOSUPP (1)
#define AT_ERRNO_NOALLOW (2)
#define AT_ERRNO_PARA_VAL (5)
#define AT_ERRNO_PARA_NUM (6)
#define AT_ERRNO_SYS (8)
#define AT_CB_PRINT (0xFF)

// DECLARE LOCAL FUNCTIONS
void set_new_config();
int hex2bin(const char *hex, uint8_t *bin, uint16_t bin_length);
void at_settings();
int at_query_mode();
int at_exec_mode(const char *str);
int at_query_region();
int at_exec_region(char *str);
int at_query_mask();
int at_exec_mask(char *str);
int at_query_joinmode();
int at_exec_joinmode(char *str);
int at_query_deveui();
int at_exec_deveui(char *str);
int at_query_appeui();
int at_exec_appeui(char *str);
int at_query_appkey();
int at_exec_appkey(char *str);
int at_query_devaddr();
int at_exec_devaddr(char *str);
int at_query_appskey();
int at_exec_appskey(char *str);
int at_query_nwkskey();
int at_exec_nwkskey(char *str);
int at_query_class();
int at_exec_class(char *str);
int at_query_join();
int at_exec_join(char *str);
int at_query_join_status();
int at_query_confirm();
int at_exec_confirm(char *str);
int at_query_datarate();
int at_exec_datarate(char *str);
int at_query_adr();
int at_exec_adr(char *str);
int at_query_txpower();
int at_exec_txpower(char *str);
int at_query_sendfreq();
int at_exec_sendfreq(char *str);
int at_exec_send(char *str);
int at_query_battery();
int at_query_rssi();
int at_query_snr();
int at_exec_reboot();
int at_query_status();
int at_exec_restore();
int at_exec_list_all();
void at_cmd_handle();
void at_serial_input(uint8_t cmd);
bool user_at_handler(char *user_cmd, uint8_t cmd_size) __attribute__((weak));
int at_query_version();

typedef struct atcmd_s
{
    const char *cmd_name;       // CMD NAME
    const char *cmd_desc;       // AT+CMD?
    int (*query_cmd)();         // AT+CMD=?
    int (*exec_cmd)(char *str); // AT+CMD=value
    int (*exec_cmd_no_para)();  // AT+CMD
} atcmd_t;

extern atcmd_t g_at_cmd_list[30];

#endif // SCAN_RAK_COMMANDS_H
