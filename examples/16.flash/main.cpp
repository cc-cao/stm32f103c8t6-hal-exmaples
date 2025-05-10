#include "generic.h"
#include "bsp/oled/OLED.h"
#include "bsp/flash.h"

int main(void) {
    OLED_Init();

    u8 read_buf[32] = {0};
    u8 write_data[4] = {0xa, 0xb, 0xc, 0xd};
    flash_page_erase(0x0800FC00, 1);
    flash_write(0x0800FC00, (uint16_t*)write_data, 2);
    flash_read(0x0800FC00, read_buf, 4);
    uint16_t flash_size;
    flash_read(0x1ffff7e0, (uint8_t*)&flash_size, 2);
    
    OLED_ShowHexNum(1, 1, read_buf[0], 2);
    OLED_ShowHexNum(1, 3, read_buf[1], 2);
    OLED_ShowHexNum(1, 5, read_buf[2], 2);
    OLED_ShowHexNum(1, 7, read_buf[3], 2);
    OLED_ShowHexNum(2, 1, flash_size, 4);
    for (;;) {}
}
