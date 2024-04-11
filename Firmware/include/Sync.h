// Sync.h
//
// Code for syncrhonising controller operations and
// standardising sample rates
//
// File created 10/4/24

#ifndef SYNC_H
#define SYNC_H

#include <stdint.h>

struct interrupt_handlers {
    // Simultaneous handlers
    // Called at every interrupt
    uint8_t n_simultaneous_handlers;
    void (**sim_handlers)(void *);
    void **sim_handler_args;

    // Sequential handlers
    // One called per interrupt, in turn
    uint8_t n_sequential_handlers;
    void (**seq_handlers)(void *);
    void **seq_handler_args;
};

void setup_interrupts(struct interrupt_handlers *handlers);

#endif