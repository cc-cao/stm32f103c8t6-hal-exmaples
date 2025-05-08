#include "generic.h"
#include "bsp/oled/OLED.h"

int main(void) {
    OLED_Init();
    OLED_ShowChar(1, 1, 'A');
    OLED_ShowString(1, 3, (char*)"HelloWorld!");
    OLED_ShowNum(2, 1, 12345, 5);

    for (;;) {
        HAL_Delay(100);
    }
}
