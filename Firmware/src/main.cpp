#include <Arduino.h>

#include <stdint.h>
#include <stdbool.h>

#include "Algorithms.h"
#include "ADC.h"
#include "Error.h"

// Specify execution mode here
#include "Mode.h"

// Pin definitions

// Analog input ports should be Port F
// Have available PF0-PF3 (should be enough - 4 input channels)


// Constant system parameters
#define PIN_ERROR 22 // PA0
#define PIN_PID_OUT_5V 12 // PB6
#define PIN_PID_OUT_10V 13 // PB7

#define PIN_PID_FB_5V 54 // PF0 (A0)
#define PIN_PID_FB_10V 55 // PF1 (A1)
#define PIN_MPPT_CURRENT 56 // PF2
#define PIN_MPPT_VOLTAGE 57 // PF3

// Analog inputs need MUX flags not pins
// Which are not the same
// Pretty similar actually but yeah 
#define MUX_FB_5V 0
#define MUX_FB_10V 1
#define MUX_MPPT_CURRENT 2
#define MUX_MPPT_VOLTAGE 3

#define PIN_MPPT_OUT 13 // PB7
#define PIN_ADC_INTTERUPT_FLAG 23 // PA1

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
  pid_update, // 5V
  pid_update, // 10V
  dummy_update, // Pad
  //mppt_wrapper,
  //mppt_wrapper
};

void *handler_args[3] = {
  (void *)&controller_5v,
  (void *)&controller_10v,
  (void *)&controller_mppt_wrapper,
};

// ADC ports
uint8_t handler_ports[3] = {
  MUX_FB_5V, // Dummy - not sure what these should be just yet
  MUX_FB_10V,
  MUX_MPPT_VOLTAGE,
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
  handler_ports[1] = MUX_MPPT_CURRENT;

  handlers[2] = mppt_wrapper;
}

void mode_disable_mppt() {
  global_handler.n_handlers = 2;
  handlers[1] = pid_update;
  handler_args[1] = (void *)&controller_10v;
  handler_ports[1] = MUX_FB_10V;

  handlers[2] = dummy_update;
}



#if defined PRODUCTION || defined DEBUG_PWM
void setup() {
  // Set up global structs here
  init_error(PIN_ERROR);

  pid_setup(&controller_5v, PIN_PID_OUT_5V, 5.0);
  pid_setup(&controller_10v, PIN_PID_OUT_10V, 5.0);
  mppt_setup(&controller_mppt);

  adc_setup(&global_handler, PIN_ADC_INTTERUPT_FLAG);
  
  // Program starts here
  adc_begin();
}

void loop() {
  if (check_fault()) {
    // Kill all execution
    // Disable all PWM output
    // Turn off machine 
    // Cry
  }
}

#endif 

#ifdef DEBUG_ADC

void setup() {
  init_error(PIN_ERROR);
  adc_setup(&global_handler, PIN_ADC_INTTERUPT_FLAG);

  Serial.begin(115200);

  adc_begin();

}

void loop() {
  // Wow
  digitalWrite(PIN_ERROR, HIGH);
  delay(1000);

  digitalWrite(PIN_ERROR, LOW);
  delay(1000);
}

#endif

#ifdef DEBUG_ADC_SAMPLE_RATE

void setup() {
  init_error(PIN_ERROR);

  // Do not actually sample - use dummy handlers
  global_handler.handlers[0] = dummy_update;
  global_handler.handlers[1] = dummy_update;
  global_handler.handlers[2] = dummy_update;

  adc_setup(&global_handler, PIN_ADC_INTTERUPT_FLAG);

  adc_begin();

  //Serial.begin(115200);
}

void loop() {
  // Ensure something is happening here to simulate a load to deal with at the same time
  digitalWrite(PIN_ERROR, HIGH);
  delay(1000);

  // Serial.write(ADCSRA);

  digitalWrite(PIN_ERROR, LOW);
  delay(1000);
}

#endif