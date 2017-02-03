#ifndef PINS_H
#define PINS_H

#include <Arduino.h>

const int MCP_INT_PIN = 2; // PE4, INT4, OC3B, pin_6
const int MCP_CS_PIN = 53; // PB0, PCINT0, SS, pin_19

// IMPORTANT: If you change any of these four pins, you must update the Rtd_Controller::setEnablePins function and the Rtd_Controller::begin function
#endif

