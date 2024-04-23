#include <Arduino.h>

#include <stdint.h>
#include <stdbool.h>

#include "Algorithms.h"
#include "ADC.h"
#include "Error.h"
#include "PWM.h"

// Specify execution mode here
#include "Mode.h"

// Pin definitions

// Analog input ports should be Port F
// Have available PF0-PF3 (should be enough - 4 input channels)


// Constant system parameters
static bool mppt_on;

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
void (*handlers[2])(uint8_t, void *) = {
  pid_update, // 5V
  pid_update, // 10V
  // dummy_update, // Pad
  //mppt_wrapper,
  //mppt_wrapper
};

void *handler_args[2] = {
  (void *)&controller_5v,
  (void *)&controller_10v,
  // (void *)&controller_mppt_wrapper,
};

// ADC ports
uint8_t handler_ports[2] = {
  MUX_FB_5V, 
  MUX_FB_10V,
  // MUX_MPPT_VOLTAGE,
};

static struct handlers global_handler = {
  .n_handlers = 2,
  .handler_in_ports = handler_ports,
  .handlers = handlers,
  .handler_args = handler_args,
};

// Only other required global
void mode_enable_mppt() {
  // global_handler.n_handlers = 2;
  handlers[1] = mppt_wrapper;
  handler_args[1] = (void *)&controller_mppt_wrapper;
  controller_5v.pwm_max = 0.9;
  // handler_ports[1] = MUX_MPPT_CURRENT;

  // handlers[2] = mppt_wrapper;

  digitalWrite(PIN_MPPT_STATUS, HIGH);
}

void mode_disable_mppt() {
  // global_handler.n_handlers = 2;
  handlers[1] = pid_update;
  handler_args[1] = (void *)&controller_10v;
  controller_5v.pwm_max = LIM_PID_OUT_MAX;
  // handler_ports[1] = MUX_FB_10V;

  // handlers[2] = dummy_update;

  digitalWrite(PIN_MPPT_STATUS, LOW);
}

// Implement little switchy thing

#if defined PRODUCTION || defined DEBUG_PWM
void setup() {
  // Set up global structs here
  pinMode(PIN_MPPT_ON, INPUT);
  pinMode(PIN_MPPT_STATUS, OUTPUT);
  mppt_on = false;

  init_error(PIN_ERROR);

  Serial.begin(115200);

// These are backwards
  pid_setup(&controller_10v, PIN_PID_OUT_10V, 2.5);
  pid_setup(&controller_5v, PIN_PID_OUT_5V, 2.5);

  // 5V setup
  controller_5v.Kd = PID_5_KD;
  controller_5v.Kp = PID_5_KP;
  controller_5v.Ki = PID_5_KI;

  controller_5v.pwm_max = LIM_PID_OUT_MAX;
  controller_10v.pwm_max = LIM_PID_OUT_MAX;

  // 5V uses a gate driver
  controller_5v.invert_pwm = true;

  // 10V also uses a gate driver but only on one MOSFET
  controller_10v.invert_pwm = false;
  controller_10v.write_pin_inverted = PIN_PID_OUT_10V_INVERTED;

  // 10V setup
  controller_10v.Kd = PID_10_KD;
  controller_10v.Kp = PID_10_KP;
  controller_10v.Ki = PID_10_KI;

  mppt_setup(&controller_mppt, PIN_PID_OUT_10V);

  pwm_set_speed();
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
    // Set everything to 1
    pwm_write(PIN_PID_OUT_10V, 0); // Unsure abt true or false
    pwm_write(PIN_PID_OUT_5V, 0);
    while (1) {
      Serial.println("Fault encountered! Terminated program");
      delay(10000);
    }
  }

  // Probe for changes on switch pin
  if (digitalRead(PIN_MPPT_ON)) {
    if (!mppt_on) {
      mppt_on = true;
      mode_enable_mppt();
    }
  } else {
    if (mppt_on) {
      mppt_on = false;
      mode_disable_mppt();
    }
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
