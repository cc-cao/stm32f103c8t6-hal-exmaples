#include "generic.h"
#include "bsp/oled/OLED.h"
#include "bsp/w25q80.h"

int main(void) {
    OLED_Init();
    w25q80_init();
    uint8_t mid;
    uint16_t did;
    w25q80_readid(&mid, &did);
    OLED_ShowString(1, 1, "MID:");
    OLED_ShowString(1, 8, "DID:");
    OLED_ShowHexNum(1, 5, mid, 2);
    OLED_ShowHexNum(1, 12, did, 4);

    uint8_t write_data[4] = {0x03, 0x02, 0x03, 0x0d};
    uint8_t read_data[4] = {0};
    w25q80_sector_erase(0x001000);
    w25q80_page_program(0x001008, write_data, 4);
    OLED_ShowString(2, 1, "W:");
    OLED_ShowHexNum(2, 4, write_data[0], 2);
    OLED_ShowHexNum(2, 7, write_data[1], 2);
    OLED_ShowHexNum(2, 10, write_data[2], 2);
    OLED_ShowHexNum(2, 13, write_data[3], 2);

    w25q80_read_data(0x001008, read_data, 4);
    OLED_ShowString(3, 1, "R:");
    OLED_ShowHexNum(3, 4, read_data[0], 2);
    OLED_ShowHexNum(3, 7, read_data[1], 2);
    OLED_ShowHexNum(3, 10, read_data[2], 2);
    OLED_ShowHexNum(3, 13, read_data[3], 2);
    for (;;) {
        HAL_Delay(100);
    }
}
