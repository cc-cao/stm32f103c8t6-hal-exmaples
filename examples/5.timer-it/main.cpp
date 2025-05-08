#include "generic.h"
#include "bsp/oled/OLED.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim1;
u32 num_counter;
u32 num1_counter;

// 1. 定时器中断
void tim2_init() {
    __HAL_RCC_TIM2_CLK_ENABLE();

    htim2.Instance = TIM2;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Prescaler = 72 - 1;
    htim2.Init.Period = 1000 - 1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim2);
    HAL_NVIC_SetPriority(TIM2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    HAL_TIM_Base_Start_IT(&htim2);
}

// 2. 外部时钟源定时器
void tim1_init() {
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio_def = {
        .Pin = GPIO_PIN_12,
        .Mode = GPIO_MODE_AF_INPUT,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_HIGH,
    };
    HAL_GPIO_Init(GPIOA, &gpio_def);

    htim1.Instance = TIM1;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Prescaler = 0;
    htim1.Init.Period = 10 - 1;
    htim1.Init.AutoReloadPreload = 0;
    HAL_TIM_Base_Init(&htim1);

    TIM_ClockConfigTypeDef tcd = {
        .ClockSource = TIM_CLOCKSOURCE_ETRMODE1,
        .ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED,
        .ClockPrescaler = TIM_ETRPRESCALER_DIV1,
        .ClockFilter = 0x00,
    };
    HAL_TIM_ConfigClockSource(&htim1, &tcd);

    HAL_NVIC_SetPriority(TIM1_UP_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
    HAL_TIM_Base_Start_IT(&htim1);
}

int main(void) {
    OLED_Init();
    OLED_ShowString(1, 1, "counter:");
    OLED_ShowString(2, 1, "cnt:");
    OLED_ShowString(3, 1, "counter1:");
    tim2_init();
    tim1_init();

    for (;;) {
        // HAL_Delay(100);
        OLED_ShowNum(1, 9, num_counter, 4);
        OLED_ShowNum(2, 5, __HAL_TIM_GET_COUNTER(&htim1), 4);
        OLED_ShowNum(3, 11, num1_counter, 4);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM2) {
        num_counter++;
    } else if (htim->Instance == TIM1) {
        num1_counter++;
    }
}


extern "C" void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim2);
}

extern "C" void TIM1_UP_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim1);
}
