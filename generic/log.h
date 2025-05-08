#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f1xx_hal.h"
#include "bsp/usart.h"

#ifdef USE_PRINT
#define log_init usart1_init
#define log(fmt, ...) printf("[%06ld] %s(%d): " fmt "\n", HAL_GetTick(), __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define log_init()
#define log(fmt, ...)
#endif

#ifdef __cplusplus
}
#endif
