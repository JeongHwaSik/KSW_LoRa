#ifndef SCANIN_H
#define SCANIN_H

// DEBUG
#define APP_DEBUG 1

// INCLUDE EXTERNAL LIBRARIES
#include <Arduino.h>
#include <LoRaWan-Arduino.h>
#include <mbed.h>
#include <rtos.h>
#include <multicore.h>
#include <time.h>
#include <Wire.h>

// INCLUDE LOCAL LIBRARIES
#include "debug.h"
#include "lora.h"
#include "flash.h"
#include "battery.h"
#include "serial.h"
#include "commands.h"
#include "handlers.h"

// EXPOSE LOCAL VARIABLES
extern osThreadId loop_thread;
extern TimerEvent_t app_timer;
extern uint8_t g_sw_ver_1; // major version increase on API change / not backwards compatible
extern uint8_t g_sw_ver_2; // minor version increase on API change / backward compatible
extern uint8_t g_sw_ver_3; // patch version increase on bugfix, no affect on API
extern bool send_flag;

void scanin_init();
void scanin_loop();
void create_at_command(const char *cmd_name, const char *cmd_desc, int (*function)());

#endif // SCANIN_H
