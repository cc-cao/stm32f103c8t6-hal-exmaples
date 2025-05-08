#include "generic.h"
#include "bsp/oled/OLED.h"

int red_counter;

void red_sensor_init() {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio_def = {
        .Pin = GPIO_PIN_14,
        .Mode = GPIO_MODE_IT_RISING,
        .Pull = GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_HIGH,
    };
    HAL_GPIO_Init(GPIOB, &gpio_def);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

int main(void) {
    OLED_Init();
    red_sensor_init();

    OLED_ShowString(1, 1, "red counter:");
    for (;;) {
        OLED_ShowNum(1, 13, red_counter, 4);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
    if (pin == GPIO_PIN_14) {
        red_counter++;
    }
}
