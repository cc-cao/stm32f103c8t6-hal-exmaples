#include "generic.h"
#include "bsp/can.h"
#include "bsp/key.h"
#include "log.h"

void loop_test() {
    u8 kn; 
    CANMsg tx_msg = {
        .id = 0x123,
        .ide = CAN_ID_STD,
        .rtr = CAN_RTR_DATA,
        .len = 2,
        .data = {0x11, 0x22},
    };

    for (;;) {
        kn = key_getnum();
        if (kn == 1) {
            can_write(&tx_msg);
            tx_msg.data[0] += 1;
            tx_msg.data[1] += 2; 
        }

        if (can_rx_flag() > 0) {
            CANMsg rx_msg;
            can_read(&rx_msg);
            log("can read: id: %#02X rtr: %u len: %u data: %#02X %#02x", rx_msg.id, rx_msg.rtr, rx_msg.len, rx_msg.data[0], rx_msg.data[1]);
        }
    }
}

void frame_type_test() {
    u8 kn; 
    CANMsg tx_msg[] = {
        {0x123, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x12345678, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x13, 0x14}},
        {0x124, CAN_ID_STD, CAN_RTR_REMOTE, 0, {0}},
        {0x02345678, CAN_ID_EXT, CAN_RTR_REMOTE, 0, {0}},
    };
    u8 index = 0;

    for (;;) {
        kn = key_getnum();
        if (kn == 1) {
            can_write(&tx_msg[index]);
            index = (index + 1) % (sizeof(tx_msg) / sizeof(tx_msg[0])); 
        }

        if (can_rx_flag() > 0) {
            CANMsg rx_msg;
            can_read(&rx_msg);
            log("can read: id: %#02X ide: %u rtr: %u len: %u data: %#02X %#02x", rx_msg.id, rx_msg.ide, rx_msg.rtr, rx_msg.len, rx_msg.data[0], rx_msg.data[1]);
        }
    }
}

void filter_test() {
    CAN_FilterTypeDef sFilterConfig;
    #define FILTER_MODE 5
#if FILTER_MODE == 1  // 16位列表模式
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO0;
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    // 配置过滤器 
    sFilterConfig.FilterIdHigh = 0x234 << 5;
    sFilterConfig.FilterIdLow = 0x345 << 5;
    sFilterConfig.FilterMaskIdHigh = 0x567 << 5;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    can_set_filter(&sFilterConfig);

    CANMsg tx_msg[] = {
        {0x123, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x234, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x345, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x456, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x567, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x678, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
    }
#elif FILTER_MODE == 2 // 16位掩码模式
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO0;
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    // 配置过滤器 
    sFilterConfig.FilterIdHigh = 0x200 << 5;
    sFilterConfig.FilterIdLow = 0x320 << 5;
    sFilterConfig.FilterMaskIdHigh = 0x700 << 5;
    sFilterConfig.FilterMaskIdLow = 0x720 << 5;
    can_set_filter(&sFilterConfig);

    CANMsg tx_msg[] = {
        {0x100, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x101, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x1EF, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x1FF, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},

        {0x200, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x201, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x2EF, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x2FF, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},

        {0x310, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x311, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x31E, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x31F, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},

        {0x320, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x321, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x32E, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x32F, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
    };
#elif FILTER_MODE == 3 // 32位列表模式
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO0;
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    // 配置过滤器 
    sFilterConfig.FilterIdHigh = 0x123 << 5;
    sFilterConfig.FilterIdLow = 0x0;
    sFilterConfig.FilterMaskIdHigh = (0x12345678 >> 13) & 0xffff;
    sFilterConfig.FilterMaskIdLow = ((0x12345678 << 3) & 0xffff) | 0x4;
    can_set_filter(&sFilterConfig);

    CANMsg tx_msg[] = {
        {0x123, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x234, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x345, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x456, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x12345678, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x0789ABCD, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
    };
#elif FILTER_MODE == 4 // 32位掩码模式
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO0;
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    // 配置过滤器 
    sFilterConfig.FilterIdHigh = (0x12345600 >> 13) & 0xffff;
    sFilterConfig.FilterIdLow = ((0x12345600 << 3) & 0xffff) | 0x4;
    sFilterConfig.FilterMaskIdHigh = (0x1FFFFF00 >> 13) & 0xffff;
    sFilterConfig.FilterMaskIdLow = ((0x1FFFFF00 << 3) & 0xffff) | 0x4 | 0x2;
    can_set_filter(&sFilterConfig);

    CANMsg tx_msg[] = {
        {0x12345600, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x12345601, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x123456FE, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x123456FF, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},

        {0x0789AB00, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x0789AB01, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x0789ABFE, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x0789ABFF, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
    };
#elif FILTER_MODE == 5 // 32位掩码模式 只要遥控帧
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO0;
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    // 配置过滤器 
    sFilterConfig.FilterIdHigh = 0x0;
    sFilterConfig.FilterIdLow = 0x2;
    sFilterConfig.FilterMaskIdHigh = 0x0;
    sFilterConfig.FilterMaskIdLow = 0x2;
    can_set_filter(&sFilterConfig);

    CANMsg tx_msg[] = {
        {0x123, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x234, CAN_ID_STD, CAN_RTR_REMOTE, 2, {0x11, 0x12}},
        {0x345, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x456, CAN_ID_STD, CAN_RTR_REMOTE, 2, {0x11, 0x12}},

        {0x0789AB00, CAN_ID_EXT, CAN_RTR_REMOTE, 2, {0x11, 0x12}},
        {0x0789AB01, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
        {0x0789ABFE, CAN_ID_EXT, CAN_RTR_REMOTE, 2, {0x11, 0x12}},
        {0x0789ABFF, CAN_ID_EXT, CAN_RTR_DATA, 2, {0x11, 0x12}},
    };
#endif

    u8 kn; 
    u8 index = 0;

    for (;;) {
        kn = key_getnum();
        if (kn == 1) {
            can_write(&tx_msg[index]);
            index = (index + 1) % (sizeof(tx_msg) / sizeof(tx_msg[0])); 
        }

        if (can_rx_flag() > 0) {
            CANMsg rx_msg;
            can_read(&rx_msg);
            log("can read: id: %#02X ide: %u rtr: %u len: %u data: %#02X %#02x", rx_msg.id, rx_msg.ide, rx_msg.rtr, rx_msg.len, rx_msg.data[0], rx_msg.data[1]);
        }
    }
}

void isr_test() {
    can_isr_enable();
    u8 kn = 0;
    CANMsg tx_msg = {0x123, CAN_ID_STD, CAN_RTR_DATA, 2, {0x11, 0x12}};

    for (;;) {
        kn = key_getnum();
        if (kn == 1) {
            can_write(&tx_msg);
        } 

        if (can_get_rx_pending() > 0) {
            CANMsg rx_msg;
            can_read(&rx_msg);
            log("can read: id: %#02X ide: %u rtr: %u len: %u data: %#02X %#02x", rx_msg.id, rx_msg.ide, rx_msg.rtr, rx_msg.len, rx_msg.data[0], rx_msg.data[1]);
        }
    }
}

int main(void) {
    key_init();
    can_init();

    // loop_test();
    // frame_type_test();
    // filter_test();
    isr_test();
}
