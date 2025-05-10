#include "generic.h"
#include "bsp/oled/OLED.h"
#include "bsp/key.h"

RTC_HandleTypeDef hrtc;

void bkp_test() {
    __HAL_RCC_BKP_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_PWR_EnableBkUpAccess();
    uint16_t data[2] = {0x1234, 0xabcd};
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, data[0]);
    HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, data[1]);

    for (;;) {
        u8 kn = key_getnum();
        if (kn == 1) {
            data[0]++;
            data[1]++;
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, data[0]);
            HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, data[1]);
        }

        OLED_ShowHexNum(1, 1, HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1), 4);
        OLED_ShowHexNum(2, 1, HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2), 4);
        HAL_Delay(100);
    }
}

void rtc_test() {
    __HAL_RCC_BKP_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_LSE_CONFIG(RCC_LSE_ON);
    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
    
    HAL_PWR_EnableBkUpAccess();
    hrtc.Instance = RTC;
    hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
    HAL_RTC_Init(&hrtc);

    RTC_TimeTypeDef st_time = {0};
    RTC_DateTypeDef st_date = {0};

    st_time.Hours = 18;
    st_time.Minutes = 55;
    st_time.Seconds = 30;
    HAL_RTC_SetTime(&hrtc, &st_time, RTC_FORMAT_BCD);

    st_date.WeekDay = RTC_WEEKDAY_SATURDAY;
    st_date.Month = RTC_MONTH_MAY;
    st_date.Year = 25; 
    st_date.Date = 10;
    HAL_RTC_SetDate(&hrtc, &st_date, RTC_FORMAT_BCD);
    HAL_RTC_WaitForSynchro(&hrtc);
    
    for (;;) {
        RTC_TimeTypeDef st_time = {0};
        HAL_RTC_GetTime(&hrtc, &st_time, RTC_FORMAT_BCD);
        HAL_RTC_GetDate(&hrtc, &st_date, RTC_FORMAT_BCD);

        OLED_ShowNum(1, 1, st_date.Year + 2000, 4);
        OLED_ShowNum(1, 6, st_date.Month, 2);
        OLED_ShowNum(1, 9, st_date.Date, 2);

        OLED_ShowNum(2, 1, st_time.Hours, 2);
        OLED_ShowNum(2, 4, st_time.Minutes, 2);
        OLED_ShowNum(2, 7, st_time.Seconds, 2);
    }
}

int main(void) {
    OLED_Init();
    key_init();

    // bkp_test();
    rtc_test();
}
