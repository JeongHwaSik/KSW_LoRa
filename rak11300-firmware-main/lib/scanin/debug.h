#ifndef SCAN_RAK_DEBUG_H
#define SCAN_RAK_DEBUG_H

// IS APP_DEBUG TRUE
#if APP_DEBUG > 0
#define APP_LOG(tag, ...)                \
    do                                   \
    {                                    \
        if (tag)                         \
            Serial.printf("[%s] ", tag); \
        Serial.printf(__VA_ARGS__);      \
        Serial.printf("\n");             \
    } while (0)
#else
#define APP_LOG(...)
#endif

// PRINT ON TWO SERIAL PORTS
#define DualSerial(...)             \
    do                              \
    {                               \
        Serial.printf(__VA_ARGS__); \
    } while (0)

#endif // SCAN_RAK_DEBUG_H