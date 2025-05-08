#include "generic.h"
#include "bsp/pwm.h"
#include "bsp/oled/OLED.h"
#include "bsp/ic.h"

int main(void) {
    OLED_Init();
    pwm_init(720 - 1, 100 - 1);
    ic_init();
    pwm_set_compare(TIM_CHANNEL_1, 50);

    OLED_ShowString(1, 1, "freq:");

    for (;;) {
        OLED_ShowNum(1, 6, ic_get_freq(), 6);
    }
}

