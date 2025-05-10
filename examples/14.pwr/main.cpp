#include "generic.h"
#include "bsp/oled/OLED.h"


void sleep_mode() {
    OLED_ShowString(1, 1, "go to sleep");
    SysTick->CTRL &=(~SysTick_CTRL_ENABLE_Msk);
    HAL_PWR_EnterSLEEPMode(0, PWR_SLEEPENTRY_WFI);

    SysTick->CTRL |=(SysTick_CTRL_ENABLE_Msk);
    OLED_Clear();
    OLED_ShowString(1, 1, "sleep wakeup");
}

void stop_mode() {    
    OLED_ShowString(1, 1, "go to stop");
    SysTick->CTRL &=(~SysTick_CTRL_ENABLE_Msk);
    HAL_PWR_EnterSTOPMode(0, PWR_STOPENTRY_WFI);
    SystemClock_Config();
    SystemCoreClockUpdate();
    SysTick->CTRL &=(~SysTick_CTRL_ENABLE_Msk);
    OLED_Clear();
    OLED_ShowString(1, 1, "stop wakeup");
    OLED_ShowNum(2, 1, SystemCoreClock, 8);
}

void standby_mode() {
    OLED_ShowString(1, 1, "go to standby");
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU); 
    HAL_Delay(1000);
    OLED_Clear();
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
    HAL_PWR_EnterSTANDBYMode();
}

int main(void) {
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio_def = {
        .Pin = GPIO_PIN_1,
        .Mode = GPIO_MODE_IT_FALLING,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_HIGH,
    };
    HAL_GPIO_Init(GPIOB, &gpio_def);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    OLED_Init();

    // sleep_mode();
    // stop_mode();
    standby_mode();

    for (;;) {
    }
}
