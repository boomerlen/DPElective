// Algorithms.h
//
// Declarations of algorithms to be implemented
//
// DP Elective
// Created 9/4/2024

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdint.h>

struct pid_controller {
    uint8_t sample_pin;
    uint8_t write_pin;
    uint8_t id;
};

struct protection {
    uint8_t sample_pin;
    uint8_t max;
};

// Dodgy casts - pass to interrupt handlers
void pid_update(void *pid);

void check_protection(void *prot);

#endif