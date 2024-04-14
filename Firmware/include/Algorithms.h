// Algorithms.h
//
// Declarations of algorithms to be implemented
//
// DP Elective
// Created 9/4/2024

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdint.h>

// Experimentally calibrated (or educatedly guessed) integrator anti-windup
#define LIM_PID_INTEGRATOR_MIN 0.0
#define LIM_PID_INTEGRATOR_MAX 1.0
#define LIM_PID_OUT_MIN 0.1
#define LIM_PID_OUT_MAX 4.5

// Max voltage at any point in converter
#define LIM_OVERVOLTAGE 25.0

// Hugo fill in one day probably
struct pid_controller {
    uint8_t write_pin;
    
    float reference;

    float Kp;
    float Ki;
    float Kd;

    float last_error;
    float last_integral;
    float last_derivative;

    // Time constant of differentiator LPF
    float filter_tau;
};

// Riley fill in
struct mppt_controller {

};

// Internals
struct mppt_wrapper {
    bool current_set;
    uint8_t current;

    struct mppt_controller *controller;
};

// Dodgy casts - pass to interrupt handlers
void pid_update(uint8_t sample, void *pid);
void pid_setup(struct pid_controller *pid);

// For Riley to fill in
void mppt_update(uint8_t sample_current, uint8_t sample_voltage, struct mppt_controller *mppt);
void mppt_setup(struct mppt_controller *mppt);

// To not break the handler calling system 
void mppt_wrapper(uint8_t sample, void *mppt_wrapper);

// NOP to avoid speeding up controllers when not in MPPT mode

void dummy_update(uint8_t sample, void *null_ptr);

#endif