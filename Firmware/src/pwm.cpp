#include "PWM.h"

#include <Arduino.h>
#include <stdbool.h>

void pwm_set_speed() {
    // Just do this for like every timer why not
    // Idk which ones I need honestly might just trial and error this shit
    TCCR0B &= !0x7;
    TCCR0B |= 1 << CS01;

    TCCR1B &= !0x07;
    TCCR1B |= 1 << CS01;

    TCCR2B &= !0x07;
    TCCR2B |= 1 << CS01;

    TCCR3B &= !0x07;
    TCCR3B |= 1 << CS01;

    TCCR4B &= !0x07;
    TCCR4B |= 1 << CS01;

    TCCR4B &= !0x07;
    TCCR4B |= 1 << CS01;

    TCCR4B &= !0x07;
    TCCR4B |= 1 << CS01;
}

void pwm_write(uint8_t pin, uint8_t pwm, bool inverting) {
    // Glance inside analogWrite clarifies that it is fine to use - definitely uses
    // hardware timers and configures them properly for multiple simultaneous analog writes
    uint8_t pwm_use;
    if (inverting) {
        pwm_use = 255 - pwm;
    } else {
        pwm_use = pwm;
    }
    analogWrite(pin, pwm_use);
}