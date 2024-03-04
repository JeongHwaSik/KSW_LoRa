#include "scanin.h"

uint8_t g_sw_ver_1 = 1; // major version increase on API change / not backwards compatible
uint8_t g_sw_ver_2 = 0; // minor version increase on API change / backward compatible
uint8_t g_sw_ver_3 = 0; // patch version increase on bugfix, no affect on API

osThreadId loop_thread = nullptr;
TimerEvent_t app_timer;
bool send_flag = false;

void scanin_init()
{
    // Call function to initialize flash memory
    init_flash();
    // Call function to initialize battery
    init_batt();
    // Check if APP_DEBUG is greater than 0, and if so, execute the following code block
#if APP_DEBUG > 0
    // Wait for Serial to be available

    // while (!Serial)
    //     ;

    delay(1000);

    // Call function to print headers
    headers();
    // Call function to initialize serial task
    init_serial_task();
#endif

    // Call function to initialize LoRaWAN communication
    init_lorawan();
}

void scanin_loop()
{
    loop_thread = osThreadGetId();       // Get the ID of the current thread
    osEvent event = osSignalWait(0, 10); // Wait for a signal to be received

    if (event.status == osEventSignal)
    { // Check if a signal was received

        if ((event.value.signals & SIGNAL_JOIN) == SIGNAL_JOIN)
        {                // Check if the JOIN signal was received
            lora_join(); // Call the lora_join function
        }
        if ((event.value.signals & SIGNAL_JOIN_SUCCESS) == SIGNAL_JOIN_SUCCESS)
        {                  // Check if the JOIN_SUCCESS signal was received
            lora_joined(); // Call the lora_joined function
        }
        if ((event.value.signals & SIGNAL_JOIN_FAIL) == SIGNAL_JOIN_FAIL)
        {                       // Check if the JOIN_FAIL signal was received
            lora_join_failed(); // Call the lora_join_failed function
        }
        if ((event.value.signals & SIGNAL_UNCONF_TX) == SIGNAL_UNCONF_TX)
        {                          // Check if the UNCONF_TX signal was received
            lora_tx_unconfirmed(); // Call the lora_tx_unconfirmed function
        }
        if ((event.value.signals & SIGNAL_CONF_TX_ACK) == SIGNAL_CONF_TX_ACK)
        {                        // Check if the CONF_TX_ACK signal was received
            lora_tx_confirmed(); // Call the lora_tx_confirmed function
        }
        if ((event.value.signals & SIGNAL_CONF_TX_NAK) == SIGNAL_CONF_TX_NAK)
        {                                  // Check if the CONF_TX_NAK signal was received
            lora_tx_confirmation_failed(); // Call the lora_tx_confirmation_failed function
        }
        if ((event.value.signals & SIGNAL_RX) == SIGNAL_RX)
        {                      // Check if the RX signal was received
            lora_rx_success(); // Call the lora_rx_success function
        }
        if ((event.value.signals & SIGNAL_SEND) == SIGNAL_SEND)
        { // Check if the SEND signal was received
            send_flag = true;
        }
    }
}

void create_at_command(const char *cmd_name, const char *cmd_desc, int (*function)())
{
    atcmd_t command = {cmd_name, cmd_desc, function, nullptr, function};
    g_at_cmd_list[27] = command;
}
