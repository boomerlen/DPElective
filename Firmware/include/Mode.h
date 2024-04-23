#ifndef MODE_H
#define MODE_H

#define PRODUCTION
// #define DEBUG_ADC
// #define DEBUG_ADC_SAMPLE_RATE
// #define DEBUG_PWM

// Controller gains!!
#define PID_5_KP 0.9
#define PID_5_KI 2.5
#define PID_5_KD 0.01

#define PID_10_KP 0.9
#define PID_10_KI 2.5
#define PID_10_KD 0.01

// Experimentally calibrated (or educatedly guessed) integrator anti-windup
// Currently just random numbers
#define LIM_PID_INTEGRATOR_MIN 0.0
#define LIM_PID_INTEGRATOR_MAX 1.0

// PID output saturation duty cycle
#define LIM_PID_OUT_MIN 0.1
#define LIM_PID_OUT_MAX 0.65

// Max voltage at any point in converter
// Note that this will be different for the different
// PIDs because they have different voltage dividers
// Nevermind - just going to use saturation of the ADC as sign of bad
#define LIM_OVERVOLTAGE 4.9

// And every constant we use too why not
#define PIN_ERROR 22 // PA0
#define PIN_PID_OUT_5V 12 // PB6
#define PIN_PID_OUT_10V 13 // PB7
#define PIN_PID_OUT_10V_INVERTED 11 // PB5

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

#define PIN_MPPT_ON 25
#define PIN_MPPT_FLAG 26
#define PIN_MPPT_STATUS 24

#endif