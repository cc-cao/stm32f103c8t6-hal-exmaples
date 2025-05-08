#include "i2c.h"
#include "generic.h"

#define GPIOX   GPIOB
#define SCL_PIN GPIO_PIN_10
#define SDA_PIN GPIO_PIN_11

#define SCL(x)                                               \
    do {                                                     \
        HAL_GPIO_WritePin(GPIOX, SCL_PIN, (GPIO_PinState)x); \
        delay_us(10);\
    } while (0)
#define SDA(x)                                               \
    do {                                                     \
        HAL_GPIO_WritePin(GPIOX, SDA_PIN, (GPIO_PinState)x); \
        delay_us(10);\
    } while (0)
#define SCL_V HAL_GPIO_ReadPin(GPIOX, SCL_PIN)
#define SDA_V HAL_GPIO_ReadPin(GPIOX, SDA_PIN)

void i2c_init() {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio_def = {
        .Pin = SCL_PIN | SDA_PIN,
        .Mode = GPIO_MODE_OUTPUT_OD,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Pull = GPIO_PULLUP,
    };
    HAL_GPIO_Init(GPIOX, &gpio_def);

    SCL(1);
    SDA(1);
}

void i2c_start() {
    SDA(1);
    SCL(1);
    SDA(0);
    SCL(0);
}

void i2c_stop() {
    SDA(0);
    SCL(1);
    SDA(1);
}

void i2c_send_byte(uint8_t byte) {
    for (int i = 0; i < 8; ++i) {
         SDA(byte & (0x80 >> i));
         SCL(1);
         SCL(0);
    }
}

uint8_t i2c_read_byte() {
    SDA(1);
    uint8_t byte = 0x00;
    for (int i = 0; i < 8; ++i) {
        SCL(1);
        if (SDA_V == 1)
            byte |= (0x80 >> i);
        SCL(0);
    }

    return byte;
}

void i2c_send_ack(uint8_t bit) {
    SDA(bit);
    SCL(1);
    SCL(0);
}

uint8_t i2c_read_ack() {
    uint8_t bit = 0;
    SDA(1);
    SCL(1);
    bit = SDA_V;
    SCL(0);
    return bit;
}
