#include "generic.h"
#include "bsp/oled/OLED.h"
#include "bsp/adc.h"

int main(void) {
    OLED_Init();
    adc_init();
    OLED_ShowString(1, 1, "adc:");
    OLED_ShowString(2, 1, "voltage:");
    OLED_ShowString(3, 1, "adc1:");
    for (;;) {
        // uint32_t res = adc_read(ADC_CHANNEL_0);
        uint16_t res = adc_dma[0];
        float vol = res * 3.3 / 4095;
        uint16_t vol_i = (int)vol;
        uint16_t vol_f = (int)((vol - vol_i) * 100);   
        OLED_ShowNum(1, 5, res, 4);
        OLED_ShowNum(2, 9, vol_i, 1);
        OLED_ShowChar(2, 10, '.');
        OLED_ShowNum(2, 11, vol_f, 2);
        // OLED_ShowNum(3, 6, adc_read(ADC_CHANNEL_1), 4);
        OLED_ShowNum(3, 6, adc_dma[1], 4);
        HAL_Delay(100);
    }
}
