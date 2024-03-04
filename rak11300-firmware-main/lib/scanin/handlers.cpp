#include "scanin.h"

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday",
                             "Wednesday", "Thursday", "Friday",
                             "Saturday"};

/**
 * @brief Initiates the process for joining the LoRa network.
 *
 * @note This function initiates the LoRa join process and logs the start of the join.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void lora_join()
{
    APP_LOG("APP", "Start Join");
    // init_lorawan();
    //  digitalWrite(LED_BLUE, HIGH);
}

/**
 * @brief Handles the scenario when the device successfully joins the LoRa network.
 *
 * @note This function logs the successful join, sends the software version information,
 * and handles potential errors in the send operation.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void lora_joined()
{
    // Print "AT+JOIN=SUCCESS" to DualSerial
    Serial.printf("AT+JOIN=SUCCESS\n");
    // Uncomment the following line to turn off the blue LED when the device joins the network
    // digitalWrite(LED_BLUE, LOW);

    // Create an array to store the software version information
    uint8_t m_lora_app_data[3] = {g_sw_ver_1, g_sw_ver_2, g_sw_ver_3};

    // Send the software version information over LoRa
    lmh_error_status result = send_lora_packet(m_lora_app_data, 3, 3);

    // Check the result of the send operation
    switch (result)
    {
    case LMH_SUCCESS:
        // Print a message to the log indicating the packet was sent successfully
        APP_LOG("APP", "Packet queued successfully");
        break;
    case LMH_BUSY:
        // Print a message to the log indicating the LoRa transceiver is busy and to try again later
        APP_LOG("APP", "LoRa transceiver is busy, retry in next cycle!");
        break;
    case LMH_ERROR:
        // Print a message to the log indicating there was an error sending the packet
        APP_LOG("APP", "Packet error, too big to send with current DR");
        // Set the data rate to 4
        lmh_datarate_set(DR_4, false);
        // Print a message to the log indicating that the packet will be resent with DR 4
        APP_LOG("APP", "Trigger resending the packet with DR 4");
        // Signal the loop thread to send the packet again
        osSignalSet(loop_thread, SIGNAL_SEND);
        break;
    }
}

/**
 * @brief Handles the scenario when the device fails to join the LoRa network.
 *
 * @note This function sends a failure message over DualSerial.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void lora_join_failed()
{
    DualSerial("AT+JOIN=FAIL\n");
    // digitalWrite(LED_BLUE, LOW);
}

/**
 * @brief Handles the scenario when a transmitted packet is confirmed.
 *
 * @note This function sends a confirmation message over DualSerial.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void lora_tx_confirmed()
{
    DualSerial("CONFIRMED\n");
    // digitalWrite(LED_BLUE, LOW);
}

/**
 * @brief Handles the scenario when a transmitted packet is unconfirmed but sent successfully.
 *
 * @note This function sends a success message over DualSerial.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void lora_tx_unconfirmed()
{
    DualSerial("AT+SEND=SUCCESS\n");
    // digitalWrite(LED_BLUE, LOW);
}

/**
 * @brief Handles the scenario when the confirmation for a transmitted packet fails.
 *
 * @note This function sends a failure message over DualSerial.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void lora_tx_confirmation_failed()
{
    DualSerial("AT+SEND=FAIL\n");
    // digitalWrite(LED_BLUE, LOW);
}

/**
 * @brief Handles the scenario when a packet is received successfully.
 *
 * @note This function lZogs the received data and its metadata, prints the received data
 * in a specific format, and controls an LED based on the received bytes.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void lora_rx_success()
{
    // Log the received data information
    APP_LOG("APP", "RX finished %d bytes, RSSI %d, SNR %d\n",
            g_rx_data_len, g_last_rssi, g_last_snr);

    // Print the received data information in a specific format
    DualSerial("RX:%d:%d:%d:%d:", g_last_fport, g_rx_data_len, g_last_rssi,
               g_last_snr);

    // Iterate over the received data
    for (int idx = 0; idx < g_rx_data_len; idx++)
    {
        // Print each byte of the received data
        DualSerial("%d", g_rx_lora_data[idx]);

        // Check if the received byte is 0x01 or 0x00
        if (g_rx_lora_data[idx] == 0x01)
        {
            // If the received byte is 0x01, turn on the LED_BLUE
            digitalWrite(LED_BLUE, HIGH);
        }
        else if (g_rx_lora_data[idx] == 0x00)
        {
            // If the received byte is 0x00, turn off the LED_BLUE
            digitalWrite(LED_BLUE, LOW);
        }
    }
    // Print "OK" and a newline
    DualSerial("\nOK\n");
}

/**
 * @brief Sends data over the LoRa network.
 *
 * @param data Pointer to the data array to be sent.
 * @param size Size of the data array.
 * @param fport The port to send the data on.
 *
 * @note This function sends a packet over LoRa, logs the result, and handles potential errors in the send operation.
 *
 * @author Khalifa Alremeithi <kalremei@purdue.edu>
 */
void lora_tx_send(uint8_t *data, int size, int fport)
{
    send_flag = false;
    // Send the packet, store the result in 'result'
    lmh_error_status result = send_lora_packet(data, size, fport);

    // Check the result of the send operation
    switch (result)
    {
    case LMH_SUCCESS:
        // Log successful packet queue
        APP_LOG("APP", "Packet queued successful");
        break;
    case LMH_BUSY:
        // Log that the LoRa transceiver is busy and to retry in the next cycle
        APP_LOG("APP", "LoRa transceiver is busy, retry in next cycle!");
        break;
    case LMH_ERROR:
        // Log that there was an error with the packet
        APP_LOG("APP", "Packet error, too big to send with current DR");
        // Set the data rate to 4
        lmh_datarate_set(DR_4, false);
        // Log that the packet will be resent with data rate 4
        APP_LOG("APP", "Trigger resending the packet with DR 4");
        // Signal the loop thread to resend the packet
        osSignalSet(loop_thread, SIGNAL_SEND);
        break;
    }
}