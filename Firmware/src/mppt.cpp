#include "Algorithms.h"
#include "PWM.h"

#include <Arduino.h>
#include <stdbool.h>

#define MPPT_COUNTER_TOP 1024
static uint32_t mppt_counter;
static bool mppt_flag_on;

void mppt_wrapper(uint8_t sample, void *mppt_wrapper) {
    // Need to slow MPPT down quite dramatically - do this with just a counter
    struct mppt_wrapper *mppt = (struct mppt_wrapper *)mppt_wrapper;

    if (mppt_counter != MPPT_COUNTER_TOP) {
        mppt_counter++;
        return;
    }
    // Cast as correct datatype

    // Always measurement current then voltage
    if (!mppt->current_set) {
        mppt->current = sample;
        mppt->current_set = true;
        return;
    } 

    mppt_counter++;

    // Second measurement is always voltage - we are good to go
    mppt_update(mppt->current, sample, mppt->controller);

    // Clear data structure so we reuse the results
    mppt->current_set = false;
}

void mppt_update(uint8_t sample_current, uint8_t sample_voltage, struct mppt_controller *mppt) {
    // Extra debugging from hugo soz Riley
    if (mppt_flag_on) {
        mppt_flag_on = false;
        digitalWrite(PIN_MPPT_FLAG, LOW);
    } else {
        mppt_flag_on = true;
        digitalWrite(PIN_MPPT_FLAG, HIGH);
    }

    // Updating MPPT struct
    mppt->prev_curr = mppt->curr;
    mppt->prev_volt = mppt->volt;
    mppt->curr = sample_current;
    mppt->volt = sample_voltage;

    // Calculating deltas
    float delta_i = mppt->curr - mppt->prev_curr;
    float delta_v = mppt->volt - mppt->prev_volt;
    float delta_p = mppt->volt * mppt->curr - mppt->prev_volt * mppt->prev_curr;

    // Calculating step size
    uint8_t step = mppt->step_scale * abs(delta_p/delta_v);

    // Saturating step value
    if (step > 15) {
        step = 15;
    } else if (step < 1) {
        step = 0;
    }

    // Calculating new PWM value
    if (delta_v == 0) {
        if (delta_i != 0) {
            if (delta_i > 0) {
                mppt->PWM_state = mppt->PWM_state + step;
            } else {
                mppt->PWM_state = mppt->PWM_state + step;
            }
        }
    } else if (delta_i/delta_v != 0) {
        if (delta_i/delta_v > -mppt->curr/mppt->volt) {
            mppt->PWM_state = mppt->PWM_state - step;
        } else {
            mppt->PWM_state = mppt->PWM_state + step;
        }
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
    Serial.println(mppt->PWM_state);
    
    // Checking that PWM doesn't exceed maximum or minimum PWM
    if (mppt->PWM_state > 254) {
        mppt->PWM_state = 254;
    } else if (mppt->PWM_state < 1) {
        mppt->PWM_state = 1;
    }

    // Writing to PWM pin
    pwm_write(mppt->write_pin, mppt->PWM_state, mppt->invert_pwm);
}

void mppt_setup(struct mppt_controller *mppt, uint8_t pin_out) {
    // Sorry Riley just some extra garbage here
    mppt_counter = 0;
    mppt_flag_on = false;

    // Set initial values for current and voltage to 0
    mppt->curr = 0;
    mppt->prev_curr = 0;
    mppt->volt = 0;
    mppt->prev_volt = 0;
    
    // Setting write pin to output pin for load 1
    mppt->write_pin = pin_out;

    // Initialising PWM pin
    mppt->PWM_state = 10;

    // Setting invert pwm
    mppt->invert_pwm = false;

    // Setting step size for MPPT
    mppt->step_scale = 1;
}