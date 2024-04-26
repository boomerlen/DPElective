#include "Algorithms.h"
#include "PWM.h"

#include <Arduino.h>
#include <stdbool.h>

#define MPPT_COUNTER_TOP 120
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
    mppt_update(sample, mppt->controller);
    mppt_counter = 0;
}

void mppt_update(uint8_t sample_voltage, struct mppt_controller *mppt) {
    // Extra debugging from hugo soz Riley
    if (mppt_flag_on) {
        mppt_flag_on = false;
        digitalWrite(PIN_MPPT_FLAG, LOW);
    } else {
        mppt_flag_on = true;
        digitalWrite(PIN_MPPT_FLAG, HIGH);
    }

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
        step = 1;
    }

    if (mppt->PWM_curr == 10 && mppt->PWM_prev == 10) {
        step  = 5;
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
    Serial.println(delta_v/delta_D);
    //Serial.println("Voltage");
    //Serial.println(delta_v);
    //Serial.println("Step Size");
    //Serial.println(delta_D);
    //Serial.println("Output PWM");
    //Serial.println(mppt->PWM_curr);
    
    // Checking that PWM doesn't exceed maximum or minimum PWM
    if (mppt->PWM_curr > 140) {
        mppt->PWM_curr = 140;
    } else if (mppt->PWM_curr < 1) {
        mppt->PWM_curr = 1;
    }

    // Writing to PWM pin
    pwm_write(mppt->write_pin, mppt->PWM_curr, mppt->invert_pwm);
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
    mppt->PWM_curr = 10;
    mppt->PWM_prev = 10;

    // Setting invert pwm
    mppt->invert_pwm = false;

    // Setting step size for MPPT
    mppt->step_scale = 1;
}