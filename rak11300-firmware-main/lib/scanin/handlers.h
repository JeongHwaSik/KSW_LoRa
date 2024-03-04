#ifndef SCAN_RAK_HANDLERS_H
#define SCAN_RAK_HANDLERS_H

// DECLARE LOCAL FUNCTIONS
void lora_join();
void lora_joined();
void lora_join_failed();
void lora_tx_confirmed();
void lora_tx_unconfirmed();
void lora_tx_confirmation_failed();
void lora_rx_success();
void lora_tx_send(uint8_t *data, int size, int fport);

#endif // SCAN_RAK_HANDLERS_H
