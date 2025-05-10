#include "generic.h"
#include "bsp/key.h"
#include "bsp/wdg.h"
#include "bsp/oled/OLED.h"

void iwdg_demo() {
    iwdg_init();

    for (;;) {
        iwdg_feed();
        OLED_ShowString(4, 1, "Feed");
        key_getnum();

        HAL_Delay(200);
        OLED_Clear();
    }
}

void wwdg_demo() {
    wwdg_init();

     for (;;) {
         key_getnum();
         HAL_Delay(30);
         wwdg_feed();
    }
}

int main(void) {
    OLED_Init();

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
        // 独立看门狗复位
        OLED_ShowString(1, 1, "iwdg reset");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)) {
        // 窗口看门狗复位
        OLED_ShowString(2, 1, "wwdg reset");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST)) {
        // 上电复位
        OLED_ShowString(3, 1, "poweron reset");
    }
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) {
        // 软件复位（如 NVIC_SystemReset）
        OLED_ShowString(4, 1, "reboot reset");
    }
    // if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)) {
    //     // NRST 引脚外部复位
    //     OLED_ShowString(5, 1, "pin reset");
    // }
    __HAL_RCC_CLEAR_RESET_FLAGS();
    HAL_Delay(1000);
    OLED_Clear();
    // iwdg_demo();
    wwdg_demo();
}
