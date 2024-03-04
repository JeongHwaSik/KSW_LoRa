// Include necessary header files
#include "main.h"

// Forward declaration of test_command function
int test_command();
int cnt = 0;

// This function is called once when the microcontroller starts or restarts
void setup()
{
    // Initialize the serial port with a baud rate of 115200
    Serial.begin(115200);
    
    // Initialize the scanin module
    scanin_init();
    
    // Set the blue LED pin as an OUTPUT
    pinMode(LED_BLUE, OUTPUT);
    
    // Turn off the blue LED initially
    digitalWrite(LED_BLUE, LOW);
}

// This function runs in a continuous loop after the setup function
void loop()
{
    // Continuously check or process the scanin module
    scanin_loop();
    
    // Turn on the blue LED
    digitalWrite(LED_BLUE, HIGH);
    
    // Wait for 100 milliseconds
    delay(20000);
    
    // Turn off the blue LED
    digitalWrite(LED_BLUE, LOW);
    
    // Wait for another 100 milliseconds
    delay(20000);

    // Define the size of the data to be sent
    int size = 10;
    
    // Initialize the data to be sent over LoRaWAN
    uint8_t data[size] = "Hello!";
    
    // Define the fport for the LoRaWAN transmission
    int fport = 8;
    
    // Create a new AT command "+TEST" with a description and a function callback
    create_at_command("+TEST", "Test command", test_command);
    
    // Check if the send flag is set
    if (send_flag)
    {
        // Send the data over LoRaWAN
        Serial.printf("The last RSSI: %d\n", g_last_rssi);
        Serial.printf("The last SNR: %d\n", g_last_snr);
        Serial.printf("The last fport: %d\n", g_last_fport);
        Serial.printf("TX data len: %d\n", g_tx_data_len);
        Serial.printf("RX data len: %d\n", g_rx_data_len);
        Serial.printf("Join trials: %d\n", g_lorawan_settings.join_trials);
        Serial.printf("The last tPX: %d\n", g_lorawan_settings.tx_power);
        Serial.printf("The last DR: %d\n", g_lorawan_settings.data_rate);
        Serial.printf("The last subband_channels: %d\n", g_lorawan_settings.subband_channels);

        lora_tx_send(data, size, fport);
        cnt++;
        Serial.println(cnt);
    }

    // Yield function to allow other processes to execute
    // yield();
}

// This function prints "HI COMMAND !" to the serial port
int test_command()
{
    Serial.println("HI COMMAND !");
}
