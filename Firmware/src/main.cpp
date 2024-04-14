#include <Arduino.h>

#include <stdint.h>
#include <stdbool.h>

#include "Algorithms.h"
#include "ADC.h"

// Globals

// We know at compile time what this looks like so we can just
// hard code these structures as global variables without requiring dynamic memory allocation
// They must be global otherwise they will become unavailable in any function calls

// Instantiate PID controller structures
static struct pid_controller controller_5v;
static struct pid_controller controller_10v;

// MPPT structures
static struct mppt_controller controller_mppt;
static struct mppt_wrapper controller_mppt_wrapper = {
  .current_set = false,
  .current = 0,
  .controller = &controller_mppt
};

// The sequential handlers
void (*handlers[3])(uint8_t, void *) = {
  pid_update,
  pid_update,
  dummy_update,
  //mppt_wrapper,
  //mppt_wrapper
};

void *handler_args[3] = {
  (void *)&controller_5v,
  (void *)&controller_10v,
  (void *)&controller_mppt_wrapper,
};

uint8_t handler_ports[3] = {
  PORT0, // Dummy - not sure what these should be just yet
  PORT1,
  PORT2,
};

static struct handlers global_handler = {
  .n_handlers = 3,
  .handler_in_ports = handler_ports,
  .handlers = handlers,
  .handler_args = handler_args,
};

// Only other required global
void mode_enable_mppt() {
  global_handler.n_handlers = 3;
  handlers[1] = mppt_wrapper;
  handler_args[1] = (void *)&controller_mppt_wrapper;
  handler_ports[1] = PORT2;

  handlers[2] = mppt_wrapper;
}

void mode_disable_mppt() {
  global_handler.n_handlers = 2;
  handlers[1] = pid_update;
  handler_args[1] = (void *)&controller_10v;
  handler_ports[1] = PORT1;

  handlers[2] = dummy_update;
}

void setup() {
  // Set up global structs here
  pid_setup(&controller_5v);
  pid_setup(&controller_10v);
  mppt_setup(&controller_mppt);

  adc_setup(&global_handler);
  
  // Program starts here
  adc_begin();
}

void loop() {
}
