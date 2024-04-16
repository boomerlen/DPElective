// PID.cpp
//
// PID implementation
//
// Created 10/4/24

#include <Arduino.h>
#include <stdint.h>

#include "Algorithms.h"
#include "ADC.h"
#include "PWM.h"
#include "Error.h"
#include "Mode.h"

#ifdef PRODUCTION
void pid_update(uint8_t sample, void *pid) {
    // Dodgy ignore lol
    struct pid_controller *p = (struct pid_controller *)pid;

    // Convert sample to float 
    float meas = ADC_CONVERT(sample);

    // 1: protection
    if (meas > LIM_OVERVOLTAGE) {
        // Guarantee that this pin is disabled then signal fault so hopefully main can kill everything
        pwm_write(p->write_pin, 0);
        signal_fault();
        return;
    }


    // Error
    float error = p->reference - sample;

    // Product
    float product = p->Kp * error;

    // Integral
    float integral = p->last_integral + 0.5 * p->Ki * ADC_PERIOD * (error + p->last_error);

    // Clamp at limits
    if (integral > LIM_PID_INTEGRATOR_MAX) {
        integral = LIM_PID_INTEGRATOR_MAX;
    } else if (integral < LIM_PID_INTEGRATOR_MIN) {
        integral = LIM_PID_INTEGRATOR_MIN;
    }

    // Differentiator
    float derivative = 2 * p->Kd / (2 * p->filter_tau + ADC_PERIOD) * (error - p->last_error) \
            + (2 * p->filter_tau - ADC_PERIOD) / (2 * p->filter_tau + ADC_PERIOD) * p->last_derivative;

    // Result and clamp
    float result = product + integral + derivative;

    if (result > LIM_PID_OUT_MAX) {
        result = LIM_PID_OUT_MAX;
    } else if (result < LIM_PID_OUT_MIN) {
        result = LIM_PID_OUT_MIN;
    }

    // Update entries
    p->last_derivative = derivative;
    p->last_error = error;
    p->last_integral = integral;

    pwm_write(p->write_pin, DAC_CONVERT(result));
}

#endif // PRODUCTION

void pid_setup(struct pid_controller *pid, uint8_t pin_out, float reference) {
    pid->filter_tau = 1;
    pid->Kd = 0;
    pid->Kp = 0;
    pid->Ki = 0;
    pid->last_derivative = 0;
    pid->last_error = 0;
    pid->last_integral = 0;
    pid->reference = reference;
    pid->write_pin = pin_out;
}


#ifdef DEBUG_PWM

void pid_update(uint8_t sample, void *pid) {
    struct pid_controller *p = (struct pid_controller *)pid;

    analogWrite(p->write_pin, sample);
}

#endif

// Put this here because why not
void dummy_update(uint8_t sample, void *null_ptr) {

}