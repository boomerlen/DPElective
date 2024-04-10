// interrupts.cpp
//
// Interrupt code for synchronisation
//
// Created 10/4/24

#include <stdint.h>

#include <Arduino.h>

#include "Sync.h"

ISR(TIMER2_COMPA_vect) {
    // Wow happy times
}

void setup_interrupts(struct interrupt_handlers *handlers) {
    // Fun fun in the sun sun
}