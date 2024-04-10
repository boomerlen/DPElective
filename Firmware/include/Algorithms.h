// Algorithms.h
//
// Declarations of algorithms to be implemented
//
// DP Elective
// Created 9/4/2024

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdint.h>
#include <stdbool.h>

struct pid_controller {
    uint8_t sample_pin;
    uint8_t write_pin;
    uint8_t id;
};

struct protection {
    uint8_t sample_pin;
    uint8_t max;
};

void pid_update(struct pid_controller *pid);

bool check_protection(struct protection *prot);

#endif