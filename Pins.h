#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

//PE7, INT7, ICP3, EIMSK (bit 7), pin_9
// Not used, but leaving for completeness of pin definitions
// #define MCP_INT_PIN_IN INT7

 // PB0, PCINT0, SS, pin_19
#define MCP_CS_PIN 53

// PB5, PCINT5, OC1A, pin_24
#define VCU_FAULT_PIN_OUT 11

// PB6, PCINT6, OC1B, pin_25
#define PRECHARGE_PIN_OUT 12

// PB7, PCINT7, OC1C, OC0A, pin_26
#define FAN_PIN_OUT 13

// PL2, T5, pin_37
#define BPD_FAULT_PIN_IN 47

// PL3, OC5A, pin_38
#define IMD_FAULT_PIN_IN 46

// PL4, OC5B, pin_39
#define BMS_FAULT_PIN_IN 45

// PL5, OC5C, pin_40
#define ESD_DRAIN_FAULT_PIN_IN 44

// PL6, pin_41
#define DRIVER_RESET_FAULT_PIN_IN 43

// PL7, pin_42
#define MASTER_RESET_FAULT_PIN_IN 42

// PC0, A8, pin_53
#define WING_PIN_OUT 37

// PC1, A9, pin_54
#define BRAKE_LIGHT_PIN_OUT 36

// PC2, A10, pin_55
#define LOW_SIDE_MEASURE_PIN_IN 35

#define SEG_DISPLAY_REG PORTJ
#define DISP_A PJ3
#define DISP_B PJ6
#define DISP_C PJ5
#define DISP_D PJ4

// PK3, PCINT9, ADC11, A11, pin_86
#define MC_ENABLE_PIN_OUT 65

// PF0, ADC0, A0, pin_97
#define LV_MEASURE_PIN_IN 54

#endif

