#include <Arduino.h>

#include "Algorithms.h"
#include "Sync.h"

// Globals
struct interrupt_handlers handler;

void setup() {
  handler.n_sequential_handlers = 0;
  handler.n_simultaneous_handlers = 0;

  setup_interrupts(&handler); 
}

void loop() {
  // put your main code here, to run repeatedly:
}
