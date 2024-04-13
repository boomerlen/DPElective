// adc.cpp
//
// ADC Beautiful beautiful stuff
//
// CREATED ON THE 12th OF APRIL 2024 BY HOOOOGO

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>

#include <ADC.h>

static struct handlers *handlers;
static uint8_t handler_counter;

#define MUX_MASK 1 << MUX0 | 1 << MUX1 | 1 << MUX2 | 1 << MUX3 | 1 << MUX4

ISR(ADC_vect) {
    // Store status register (to avoid breaking concurrent arithmetric if there was any)
    uint8_t curr_sreg = SREG;

    // At this point - can reenable interrupts and check for interrupt collisions
    // Or could just assume this won't happen. With fast enough code it definitely won't happen
    // and I think the window is very large so all good.

    // Extract data
    uint8_t adc_result = ADCL;
    
    // Configure ADC mux to point to next port
    uint8_t next_handler_index = (handler_counter == handlers->n_handlers - 1) ? 0 : handler_counter + 1; 
    uint8_t new_mux = handlers->handler_in_ports[next_handler_index] << MUX4;
    ADMUX = (ADMUX & ~MUX_MASK) | (new_mux & MUX_MASK);

    // Start next acquisition - now have limited time to do remaining operations before next trigger
    ADCSRA |= 1 << ADSC;

    // Now divert to handler
    handlers->handlers[handler_counter](adc_result, handlers->handler_args[handler_counter]);

    // Increment handler "program counter"
    handler_counter = next_handler_index;

    // Restore sreg
    SREG = curr_sreg;
}

void adc_setup(struct handlers *h) {
    // Handler direction management
    handlers = h;
    handler_counter = 0;

    // ADC register configuration
    ADMUX |= 1 << REFS0; // REFS1:0 = 01
    ADMUX |= h->handler_in_ports[0] << MUX4; // Set port to first port

    ADCSRA |= 1 << ADEN; // Enable ADC (not same as starting conversion)
    ADCSRA |= 1 << ADIE; // Enable ADC complete interrupts

}

void adc_begin() {
    ADCSRA |= 1 << ADSC;
}

void adc_stop() {
    ADCSRA &= 0 << ADIE; // Disable ADC complete interrupts (effectively disables ADC progress)
}
