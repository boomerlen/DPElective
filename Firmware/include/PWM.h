// PWM.h
//
// PWM output
//
// Will probably end up just being analogWrite but just in case
//
// Created 14/4/24

#ifndef PWM_H
#define PWM_H

#define DAC_CONVERT(x) int(x * 256.0 / 5.0)

#include <stdint.h>

void pwm_write(uint8_t pin, uint8_t pwm);

#endif