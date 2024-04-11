// interrupts.cpp
//
// Interrupt code for synchronisation
//
// Created 10/4/24

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

#include "Sync.h"

static struct interrupt_handlers *global_handler;

static uint8_t interrupt_counter;
static bool in_interrupt;

ISR(TIMER2_COMPA_vect) {
    if (in_interrupt) {
        // Error
        return;
    }

    // Enable interrupts
    // seti
    in_interrupt = true; 

    // Always call all simultaneous handlers
    for (int i = 0; i < global_handler->n_simultaneous_handlers; i++) {
        global_handler->sim_handlers[i](global_handler->sim_handler_args[i]);
    }

    // Only call one sequential handler and then increment interrupt counter
    global_handler->seq_handlers[interrupt_counter](global_handler->seq_handler_args[interrupt_counter]);

    // Ensure we wrap around correctly
    interrupt_counter = (interrupt_counter == global_handler->n_sequential_handlers - 1) ? 0 : interrupt_counter + 1;
    in_interrupt = false;
}

void setup_interrupts(struct interrupt_handlers *handlers) {
    // Configure globals
    interrupt_counter = 0;
    in_interrupt = false;

    global_handler = handlers;

    // Actually configure the interrupt + timing
}