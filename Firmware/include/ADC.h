// ADC.h
//
// Non-blocking implementations of ADC operations
//
// Created 12/4/24

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

// Macro to convert ADC output to float in standard (5V reference) case
#define ADC_CONVERT(x) x * 5 / 256

#define ADC_PERIOD 39.0e-6

// Structure for directing ADC results
struct handlers {
    uint8_t n_handlers;
    uint8_t *handler_in_ports; // one in port per handler call
    void (**handlers)(uint8_t, void *);
    void **handler_args;
};

void adc_setup(struct handlers *h, uint8_t interrupt_triggered_flag);

void adc_begin();

void adc_stop();

#endif