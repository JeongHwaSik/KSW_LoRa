#ifndef SCAN_RAK_BATTERY_H
#define SCAN_RAK_BATTERY_H

// DEFINE LOCAL CONSTANTS
#define PIN_VBAT WB_A0
#define VBAT_MV_PER_LSB (0.806F)
#define VBAT_DIVIDER (0.6F)
#define VBAT_DIVIDER_COMP (1.846F)
#define REAL_VBAT_MV_PER_LSB (VBAT_DIVIDER_COMP * VBAT_MV_PER_LSB)

// DECLARE LOCAL FUNCTIONS
void init_batt();
float read_batt();
uint8_t mv_to_percent(float mvolts);
uint8_t get_lora_batt();

#endif // SCAN_RAK_BATTERY_H
