#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f1xx_hal.h"
#include <stdint.h>

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_send_byte(uint8_t byte);
uint8_t i2c_read_byte();
void i2c_send_ack(uint8_t bit);
uint8_t i2c_read_ack();

#ifdef __cplusplus
}
#endif
