#include "scanin.h"

/**
 * @brief   Global variable representing the battery voltage pin.
 * @note    The value is set to the pre-defined PIN_VBAT constant.
 */
uint32_t vbat_pin = PIN_VBAT;

/**
 * @brief   Initializes the battery reading settings.
 * @note    This function sets the analog read resolution to 12 bits.
 * @author  Khalifa Alremeithi <kalremei@purdue.edu>
 */
void init_batt()
{
    analogReadResolution(12);
}

/**
 * @brief   Reads the battery voltage.
 * @return  A float value representing the battery voltage in millivolts (mV).
 * @note    This function reads the raw value from the battery pin and then converts it to real voltage.
 * @author  Khalifa Alremeithi <kalremei@purdue.edu>
 */
float read_batt()
{
    float raw;
    raw = analogRead(vbat_pin);
    return raw * REAL_VBAT_MV_PER_LSB;
}

/**
 * @brief   Converts millivolts to percentage representation of battery level.
 * @param   mvolts The input voltage in millivolts.
 * @return  A uint8_t value representing the battery level as a percentage.
 * @note    The function maps the given millivolts to a percentage based on predefined voltage levels.
 * @author  Khalifa Alremeithi <kalremei@purdue.edu>
 */
uint8_t mv_to_percent(float mvolts)
{
    if (mvolts < 3300)
        return 0;

    if (mvolts < 3600)
    {
        mvolts -= 3300;
        return mvolts / 30;
    }

    if (mvolts > 4200)
    {
        return 100;
    }

    mvolts -= 3600;
    return 10 + (mvolts * 0.15F);
}

/**
 * @brief   Computes the battery level for LoRa communication.
 * @return  A uint8_t value representing the battery level for LoRa.
 * @note    The function reads the battery voltage multiple times, averages the readings, and then maps the average voltage to a percentage.
 * @author  Khalifa Alremeithi <kalremei@purdue.edu>
 */
uint8_t get_lora_batt()
{
    uint16_t read_val = 0;
    for (int i = 0; i < 10; i++)
    {
        read_val += read_batt();
    }
    return (mv_to_percent(read_val / 10) * 2.54);
}
