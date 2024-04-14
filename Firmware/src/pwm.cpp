#include "PWM.h"

#include <Arduino.h>

void pwm_write(uint8_t pin, uint8_t pwm) {
    analogWrite(pin, pwm);
}