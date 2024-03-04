#include "scanin.h"

rtos::Thread thread_handle_serial(osPriorityNormal, 4096); // thread handle for serial task
osThreadId serial_task_thread = nullptr;                   // thread id for serial task

[[noreturn]] void serial_task()
{
    serial_task_thread = osThreadGetId(); // store thread id for later use

    // check if serial is available and read all input
    if (Serial.available())
    {
        while (Serial.available() > 0)
        {
            Serial.read();
            delay(10);
        }
    }

    while (true)
    {
        // check if serial is available and process input
        if (Serial.available())
        {
            while (Serial.available() > 0)
            {
                at_serial_input(uint8_t(Serial.read()));
                delay(5);
            }
        }
        delay(100); // delay for 100ms
        yield();
    }
}

bool init_serial_task()
{
    thread_handle_serial.start(serial_task);             // start the serial task
    thread_handle_serial.set_priority(osPriorityNormal); // set priority of the thread to normal
    return true;
}

void headers()
{
    DualSerial("============================\n");
    DualSerial("RAK11300 AT Command Firmware\n");
    DualSerial("SW Version %d.%d.%d\n", g_sw_ver_1, g_sw_ver_2, g_sw_ver_3);
    DualSerial(
        "LoRa(R) is a registered trademark or service mark of Semtech Corporation or its affiliates.LoRaWAN(R) is a licensed mark.\n");
    DualSerial("============================\n");
    at_settings(); // call at_settings function
    DualSerial("============================\n");
}
