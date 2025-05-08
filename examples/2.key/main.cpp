#include "generic.h"
#include "bsp/led.h"
#include "bsp/key.h"
#include "bsp/bee.h"
#include "bsp/light_sensor.h"

int main(void) {
    led_a12_init();
    key_init();
    bee_init();
    light_sensor_init();

    for (;;) {
        u8 kn = key_getnum();
        switch (kn) {
        case 1:
            led_a1_toggle();
            break;
        case 2:
            led_a2_toggle();
            break;
        default:
            break;
        }

        GPIO_PinState lsv = light_sensor_read();
        if (lsv == GPIO_PIN_SET) {
            bee_on();
        } else {
            bee_off();
        }
    }
}
