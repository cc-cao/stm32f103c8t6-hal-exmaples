#include "can.h"

static CAN_HandleTypeDef hcan1;

void can_init() {
    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

     GPIO_InitTypeDef gpio_def = {
        .Pin = GPIO_PIN_11,
        .Mode = GPIO_MODE_AF_INPUT,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };
    HAL_GPIO_Init(GPIOA, &gpio_def);

    gpio_def.Mode = GPIO_MODE_AF_PP;
    gpio_def.Pin = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOA, &gpio_def);

    hcan1.Instance = CAN1;
    hcan1.Init.Mode = CAN_MODE_LOOPBACK;
    // 125k clk / prescale / all tq
    hcan1.Init.Prescaler = 48; 
    hcan1.Init.TimeSeg1 = CAN_BS1_2TQ;
    hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
    hcan1.Init.SyncJumpWidth = CAN_SJW_2TQ;
    hcan1.Init.AutoRetransmission = ENABLE;
    hcan1.Init.TransmitFifoPriority = ENABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.AutoWakeUp = ENABLE;
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    
    HAL_CAN_Init(&hcan1);

    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterFIFOAssignment = CAN_FilterFIFO0;
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;
    // 配置过滤器 
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    
    HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);

    HAL_CAN_Start(&hcan1);
}

void can_write(u32 id, u8 len, u8* data) {
    CAN_TxHeaderTypeDef tx_header = {0};
    uint32_t tx_mailbox = 0;

    if (len > 8) {
        len = 8;
    }

    if ((len > 0U) && (data == NULL)) {
        return;
    }

    if (HAL_CAN_GetState(&hcan1) == HAL_CAN_STATE_READY) {
        if (HAL_CAN_Start(&hcan1) != HAL_OK) {
            return;
        }
    }

    if (id <= 0x7FFU) {
        tx_header.IDE = CAN_ID_STD;
        tx_header.StdId = id;
    } else {
        tx_header.IDE = CAN_ID_EXT;
        tx_header.ExtId = (id & 0x1FFFFFFFU);
    }

    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = len;
    tx_header.TransmitGlobalTime = DISABLE;

    if (HAL_CAN_AddTxMessage(&hcan1, &tx_header, data, &tx_mailbox) != HAL_OK) {
        return;
    }

    while (HAL_CAN_IsTxMessagePending(&hcan1, tx_mailbox) != 0U) {
    }
}

u8 can_rx_flag() {
    return (u8)HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0);
}

void can_read(u32* id, u8* len, u8* data) {
    CAN_RxHeaderTypeDef pHeader;
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &pHeader, data);

    if (pHeader.IDE == CAN_ID_STD) {
        *id = pHeader.StdId;
    } else {
        *id = pHeader.ExtId;
    }

    *len = pHeader.DLC;
}
