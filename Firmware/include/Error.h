// Error.h
//
// Common code to signal that something bad has happened
//
// Created 14/4/24

#ifndef ERROR_H
#define ERROR_H

#include "ADC.h"
#include <stdbool.h>
#include <stdint.h>

// Display signal to user that fault has occured
// Also set fault flag 
void signal_fault();

// Return value of fault flag
bool check_fault();

// Initialises the error handling system
void init_error(uint8_t pin);

#endif