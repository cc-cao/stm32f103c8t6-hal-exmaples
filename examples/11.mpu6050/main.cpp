#include "generic.h"
#include "bsp/oled/OLED.h"
#include "bsp/mpu6050.h"

int main(void) {
    OLED_Init();
    mpu_init();
    u8 id = mpu_readid();
    MPUData data;
    OLED_ShowHexNum(1, 1, id, 2);
    for (;;) {
        mpu_read(&data);
        OLED_ShowSignedNum(2, 1, data.accx, 5);
        OLED_ShowSignedNum(3, 1, data.accy, 5);
        OLED_ShowSignedNum(4, 1, data.accz, 5);
        OLED_ShowSignedNum(2, 9, data.gyrox, 5);
        OLED_ShowSignedNum(3, 9, data.gyroy, 5);
        OLED_ShowSignedNum(4, 9, data.gyroz, 5);
    }
}

