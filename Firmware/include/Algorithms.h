// Algorithms.h
//
// Declarations of algorithms to be implemented
//
// DP Elective
// Created 9/4/2024

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdint.h>

// Hugo fill in one day probably
struct pid_controller {
    uint8_t write_pin;
    uint8_t id;
    float reference;
    float Kp;
    float Ki;
    float Kd;
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

#endif