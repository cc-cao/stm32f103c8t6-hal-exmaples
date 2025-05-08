#include "generic.h"
#include "bsp/pwm.h"
#include "bsp/oled/OLED.h"
#include "bsp/ic.h"

int main(void) {
    OLED_Init();
    pwm_init(720 - 1, 100 - 1);
    pwmi_init();
    pwm_set_compare(TIM_CHANNEL_1, 25);

    OLED_ShowString(1, 1, "freq:");
    OLED_ShowString(2, 1, "duty:");

    for (;;) {
        OLED_ShowNum(1, 6, ic_get_freq(), 6);
        OLED_ShowNum(2, 6, pwmi_get_duty(), 6);
    }
}
