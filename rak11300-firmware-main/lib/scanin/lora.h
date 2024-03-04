#ifndef SCAN_RAK_LORA_H
#define SCAN_RAK_LORA_H

// LORA EVENT KEYS
#define SIGNAL_UNCONF_TX 0x0001
#define SIGNAL_CONF_TX_ACK 0x0002
#define SIGNAL_CONF_TX_NAK 0x0004
#define SIGNAL_SEND 0x0008
#define SIGNAL_JOIN_SUCCESS 0x0010
#define SIGNAL_JOIN_FAIL 0x0020
#define SIGNAL_RX 0x0040
#define SIGNAL_JOIN 0x0080

// DEFINE LOCAL STRUCT
struct s_lorawan_settings
{
    uint8_t valid_mark_1 = 0xAA;
    uint8_t valid_mark_2 = 0x55;
    uint8_t node_device_eui[8] = {0x00, 0x0D, 0x75, 0xE6, 0x56, 0x4D, 0xC1, 0xF3};
    uint8_t node_app_eui[8] = {0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x02, 0x01, 0xE1};
    uint8_t node_app_key[16] = {0x2B, 0x84, 0xE0, 0xB0, 0x9B, 0x68, 0xE5, 0xCB, 0x42, 0x17, 0x6F, 0xE7, 0x53, 0xDC,
                                0xEE, 0x79};
    uint32_t node_dev_addr = 0x26021FB4;
    uint8_t node_nws_key[16] = {0x32, 0x3D, 0x15, 0x5A, 0x00, 0x0D, 0xF3, 0x35, 0x30, 0x7A, 0x16, 0xDA, 0x0C, 0x9D,
                                0xF5, 0x3F};
    uint8_t node_apps_key[16] = {0x3F, 0x6A, 0x66, 0x45, 0x9D, 0x5E, 0xDC, 0xA6, 0x3C, 0xBC, 0x46, 0x19, 0xCD, 0x61,
                                 0xA1, 0x1E};
    bool otaa_enabled = true;
    bool adr_enabled = false;
    bool public_network = true;
    bool duty_cycle_enabled = false;
    uint32_t send_repeat_time = 120000;
    uint8_t join_trials = 5;
    uint8_t tx_power = 0;
    uint8_t data_rate = 3;
    uint8_t lora_class = 0;
    uint8_t subband_channels = 1;
    bool auto_join = false;
    uint8_t app_port = 2;
    lmh_confirm confirmed_msg_enabled = LMH_UNCONFIRMED_MSG;
    uint8_t lora_region = 1;
    bool lorawan_enable = true;
    bool resetRequest = true;
};

// EXPOSE LOCAL VARIABLES
extern s_lorawan_settings g_lorawan_settings;
extern bool g_lorawan_initialized;
extern int16_t g_last_rssi;
extern int8_t g_last_snr;
extern uint8_t g_last_fport;
extern uint8_t m_lora_app_data_buffer[];
extern bool g_lpwan_has_joined;
extern uint8_t g_rx_data_len;
extern uint8_t g_rx_lora_data[];
extern uint8_t g_tx_lora_data[];
extern uint8_t g_tx_data_len;
extern char *bandwidths[];
extern char *region_names[];

// DECLARE LOCAL FUNCTIONS
bool send_lpwan_packet();
int8_t init_lorawan();
void lpwan_joined_handler();
void lpwan_join_fail_handler();
void lpwan_rx_handler(lmh_app_data_t *app_data);
void lpwan_class_confirm_handler(DeviceClass_t Class);
void lpwan_unconfirm_tx_finished();
void lpwan_confirm_tx_finished(bool result);
lmh_error_status send_lora_packet(uint8_t *data, uint8_t size, uint8_t fport);
void trigger_sending();

#endif // SCAN_RAK_LORA_H
