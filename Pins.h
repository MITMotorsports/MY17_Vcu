#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

// const int MCP_INT_PIN = INT7; //PE7, INT7, ICP3, EIMSK (bit 7), pin_9
// const int MCP_INT_PIN = 2; // PE4, INT4, OC3B, pin_6
const int MCP_CS_PIN = 53; // PB0, PCINT0, SS, pin_19

#define SEG_DISPLAY_REG PORTJ
const uint8_t DISP_A = PJ3;
const uint8_t DISP_B = PJ6;
const uint8_t DISP_C = PJ5;
const uint8_t DISP_D = PJ4;

const int VCU_SHUTDOWN_PIN = 11; // PB5, PCINT5, OC1A, pin_24
const int PRECHARGE_PIN = 12; // PB6, PCINT6, OC1B, pin_25

const int MC_ENABLE_PIN = 65; //PK3, PCINT19, ADC11/A11, pin_86

#endif

