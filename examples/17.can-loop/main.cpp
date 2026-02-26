#include "generic.h"
#include "bsp/can.h"
#include "bsp/key.h"
#include "log.h"

int main(void) {
    log_init();
    key_init();
    can_init();

    u8 kn; 
    u8 wd[] = {0x11, 0x22};

    for (;;) {
        kn = key_getnum();
        if (kn == 1) {
            can_write(0x123, 2, wd);
            wd[0] += 1;
            wd[1] += 2; 
        }

        if (can_rx_flag() > 0) {
            u32 id = 0;
            u8 len = 0;
            u8 rd[8];
            can_read(&id, &len, rd);
            log("can read: id: %#02X len: %u data: %#02X %#02x", id, len, rd[0], rd[1]);
        }
    }
}
