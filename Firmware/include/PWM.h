// PWM.h
//
// PWM output
//
// Will probably end up just being analogWrite but just in case
//
// Created 14/4/24

#ifndef PWM_H
#define PWM_H

#define DAC_CONVERT(x) (uint8_t)(x * 256.0)

#include <stdint.h>
#include <stdbool.h>

void pwm_set_speed();

void pwm_write(uint8_t pin, uint8_t pwm, bool inverting = false);

#endif