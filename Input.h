#ifndef _INPUT_H
#define _INPUT_H

#include <Arduino.h>

#include <MY17_Can_Library.h>

#include "Types.h"

typedef enum {
  FRONT_CAN_NODE_LIVENESS,
  REAR_CAN_NODE_LIVENESS,
  BMS_LIVENESS,
  DASH_LIVENESS,
  MC_LIVENESS,
  CURRENT_SENSOR_LIVENESS,
} Liveness;

void Input_initialize(Input_T *input);
void Input_fill_input(Input_T *input);

bool Input_shutdown_loop_closed(Input_T *input);

bool Input_device_alive(Input_T *input, Liveness l);
bool Input_all_devices_alive(Input_T *input);


#endif // _INPUT_H
