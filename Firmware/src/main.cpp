#include <Arduino.h>

#include "Algorithms.h"
#include "Sync.h"

// Globals

// We know at compile time what this looks like so we can just
// hard code these structures as global variables without requiring dynamic memory allocation
// They must be global otherwise they will become unavailable in any function calls

// Instantiate PID controller structures
static struct pid_controller controller_5v;
static struct pid_controller controller_10v;

// Instantiate protections
static struct protection protection_out_voltage;
static struct protection protection_in_current;

// The sequential handlers
void (*seq_handlers[2])(void *) = {
  pid_update,
  pid_update
};

void *seq_handler_args[2] = {
  (void *)&controller_5v,
  (void *)&controller_10v
};

void (*sim_handlers[2])(void *) = {
  check_protection,
  check_protection
};

void *sim_handler_args[2] = {
  (void *)&protection_out_voltage,
  (void *)&protection_in_current
};

static struct interrupt_handlers global_handler = {
  .n_simultaneous_handlers = 2,
  .sim_handlers = sim_handlers,
  .sim_handler_args = sim_handler_args,
  .n_sequential_handlers = 2,
  .seq_handlers = seq_handlers,
  .seq_handler_args = seq_handler_args,
};

// Only other required global
void mode_switch() {
  // Idea would be to replace the second function pointer in seq_handlers
  // and seq_handler_args
}

void setup() {
  // Set up global structs here

  setup_interrupts(&global_handler); 
}

void loop() {
  // put your main code here, to run repeatedly:
}
