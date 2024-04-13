#include "Algorithms.h"

#include <stdbool.h>

void mppt_wrapper(uint8_t sample, void *mppt_wrapper) {
    // Cast as correct datatype
    struct mppt_wrapper *mppt = (struct mppt_wrapper *)mppt_wrapper;

    // Always measurement current then voltage
    if (!mppt->current_set) {
        mppt->current = sample;
        mppt->current_set = true;
        return;
    } 

    // Second measurement is always voltage - we are good to go
    mppt_update(mppt->current, sample, mppt->controller);

    // Clear data structure so we reuse the results
    mppt->current_set = false;
}

void mppt_update(uint8_t sample_current, uint8_t sample_voltage, struct mppt_controller *mppt) {

}

void mppt_setup(struct mppt_controller *mppt) {

}