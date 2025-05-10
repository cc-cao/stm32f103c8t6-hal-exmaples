#include "generic.h"
#include "bsp/led.h"
#include "bsp/flash.h"
#include "bsp/oled/OLED.h"

int main(void) {
    OLED_Init();
    u32 devid = HAL_GetDEVID();
    u32 uid[3];
    uid[0] = HAL_GetUIDw0();
    uid[1] = HAL_GetUIDw1();
    uid[2] = HAL_GetUIDw2();
    uint16_t flash_size;
    flash_read(0x1ffff7e0, (uint8_t*)&flash_size, 2);
    
    log("welcom use stm32f103c8t6 !");
    log("device id: %04X", devid);
    log("device 96bit uid: %04X-%04X-%04X", uid[0], uid[1], uid[2]);
    log("device flash size: %hu kb", flash_size);

    OLED_ShowString(1, 1, "stm32f103c8t6");
    OLED_ShowString(2, 1, "id:");
    OLED_ShowHexNum(2, 4, devid, 4);
    OLED_ShowString(3, 1, "uid:");
    OLED_ShowHexNum(3, 5, uid[0], 4);
    OLED_ShowHexNum(3, 9, uid[1], 4);
    OLED_ShowHexNum(3, 13, uid[2], 4);
    OLED_ShowString(4, 1, "flash size:  kb");
    OLED_ShowNum(4, 12, flash_size, 2);

    for (;;) {
        led_toggle();
        HAL_Delay(500);
    }
}
