#include "generic.h"
#include "bsp/led.h"

int main(void) {
    u32 devid = HAL_GetDEVID();
    u32 uid[3];
    uid[0] = HAL_GetUIDw0();
    uid[1] = HAL_GetUIDw1();
    uid[2] = HAL_GetUIDw2();

    log("welcom use stm32f103c8t6 !");
    log("device id: %04X", devid);
    log("device 96bit uid: %04X-%04X-%04X", uid[0], uid[1], uid[2]);

    for (;;) {
        led_toggle();
        HAL_Delay(500);
    }
}
