#include "generic.h"
#include "bsp/pwm.h"
#include "bsp/servo.h"
#include "bsp/key.h"
#include "bsp/motor.h"

void breath_led() {
    pwm_init(720 - 1, 100 - 1);
    u8 cmp = 0;
    u8 direct = 1;

    for (;;) {
        cmp += direct;
        if (cmp == 100) {
            direct = -1;
        } else if (cmp == 0) {
            direct = 1;
        }
        pwm_set_compare(TIM_CHANNEL_1, cmp);
        HAL_Delay(10);
    }
}

void pwm_servo() {
    servo_init();
    key_init();
    float angle = 0;

    for (;;) {
        uint8_t kn = key_getnum();
        if (kn == 1) {
            angle += 30;
            if (angle > 180) {
                angle = 0;
            }
        }

        servo_set_angle(angle);
    }
}

void motor_test() {
    u8 cmp = 100;
    motor_init();
    key_init();

    for (;;) {
        u8 kn = key_getnum();
        if (kn == 1) {
            cmp += 10;
            if (cmp > 100) cmp = 0;
            motor_set_speed(cmp);
        }
    }
}

int main(void) {
    // breath_led();
    // pwm_servo();
    motor_test();
}

