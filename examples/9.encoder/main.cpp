#include "generic.h"
#include "bsp/oled/OLED.h"
#include "bsp/encoder.h"

int main(void) {
    OLED_Init();
    encoder_init();
    OLED_ShowString(1, 1, "pos:");
    OLED_ShowString(2, 1, "speed:");

    int16_t pos = 0;
    for (;;) {
        int16_t speed = encoder_get();
        pos += speed;
        OLED_ShowSignedNum(1, 5, pos, 5);
        OLED_ShowSignedNum(2, 7, speed, 5);
        HAL_Delay(1000);
    }
}
