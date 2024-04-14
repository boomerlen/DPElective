// error.cpp
//
// Simple error handling + reporting
//
// 14/4/24

#include "Error.h"

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

static bool fault;
static uint8_t fault_pin;

void init_error(uint8_t pin) {
    fault = false;
}

bool check_fault() {
    return fault;
}

void signal_fault() {
    // Should include other data
    digitalWrite(fault_pin, HIGH);
    fault = true;
}