#pragma once
#include "stm32f1xx_hal.h"
#include "utypes.h"

#ifdef __cplusplus
extern "C" {
#endif
void can_init();
void can_write(u32 id, u8 len, u8* data);
u8 can_rx_flag();
void can_read(u32* id, u8* len, u8* data);

#ifdef __cplusplus
}
#endif