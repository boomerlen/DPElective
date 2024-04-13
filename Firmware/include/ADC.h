// ADC.h
//
// Non-blocking implementations of ADC operations
//
// Created 12/4/24

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

// Structure for directing ADC results
struct handlers {
    uint8_t n_handlers;
    uint8_t *handler_in_ports; // one in port per handler call
    void (**handlers)(uint8_t, void *);
    void **handler_args;
};

void adc_setup(struct handlers *h);

void adc_begin();

void adc_stop();

#endif