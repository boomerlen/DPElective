#include "Algorithms.h"
#include "PWM.h"

#include <Arduino.h>
#include <stdbool.h>

void mppt_wrapper(uint8_t sample, void *mppt_wrapper) {
    // Cast as correct datatype
    struct mppt_wrapper *mppt = (struct mppt_wrapper *)mppt_wrapper;

    // Always measurement current then voltage
    if (!mppt->current_set) {
        mppt->current = sample;
        mppt->current_set = true;
        return;
    } 

    // Second measurement is always voltage - we are good to go
    mppt_update(mppt->current, sample, mppt->controller);

    // Clear data structure so we reuse the results
    mppt->current_set = false;
}

void mppt_update(uint8_t sample_current, uint8_t sample_voltage, struct mppt_controller *mppt) {
    // Updating MPPT struct
    mppt->prev_volt = mppt->volt;
    mppt->volt = sample_voltage;

    // Calculating deltas
    float delta_v = mppt->volt - mppt->prev_volt;
    float delta_D = mppt->PWM_curr - mppt->PWM_prev;

    // Calculating step size
    uint8_t step = mppt->step_scale * abs(delta_v/delta_D);

    // Saturating step value
    if (step > 15) {
        step = 15;
    } else if (step < 1) {
        step = 0.5;
    }

    // Calculating updated PWM
    mppt->PWM_prev = mppt->PWM_curr;
    if (delta_v/delta_D > 0) {
        mppt->PWM_curr = mppt->PWM_curr + step;
    } else {
        mppt->PWM_curr = mppt->PWM_curr - step;
    }

    // Debugging Output Statements
    Serial.println("New Sample");
    Serial.println("Current: ");
    Serial.println(mppt->curr);
    Serial.println("Voltage");
    Serial.println(mppt->volt);
    Serial.println("Step Size");
    Serial.println(step);
    Serial.println("Output PWM");
    Serial.println(mppt->PWM_curr);
    
    // Checking that PWM doesn't exceed maximum or minimum PWM
    if (mppt->PWM_curr > 254) {
        mppt->PWM_curr = 254;
    } else if (mppt->PWM_curr < 1) {
        mppt->PWM_curr = 1;
    }

    // Writing to PWM pin
    pwm_write(mppt->write_pin, mppt->PWM_curr, mppt->invert_pwm);
}

void mppt_setup(struct mppt_controller *mppt, uint8_t pin_out) {
    // Set initial values for current and voltage to 0
    mppt->curr = 0;
    mppt->prev_curr = 0;
    mppt->volt = 0;
    mppt->prev_volt = 0;
    
    // Setting write pin to output pin for load 1
    mppt->write_pin = pin_out;

    // Initialising PWM pin
    mppt->PWM_curr = 10;
    mppt->PWM_prev = 10;

    // Setting invert pwm
    mppt->invert_pwm = false;

    // Setting step size for MPPT
    mppt->step_scale = 10;
}