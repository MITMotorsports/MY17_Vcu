#ifndef _INPUT_H
#define _INPUT_H

#include <Arduino.h>

#include <MY17_Can_Library.h>

typedef struct {
  // TODO add more things we care about for CAN Node
  int16_t requested_torque;
  bool brakes_engaged;
  uint32_t last_updated;
} Can_Node_Input_T;

typedef struct {
  // TODO add more things we care about for dash
  Can_Dash_RequestID_T request_type;
  uint32_t request_timestamp;
  uint32_t last_updated;
} Dash_Input_T;

typedef struct {
  // TODO add more things we care about for BMS
  Can_Bms_StateID_T state;
  uint32_t last_updated;
} Bms_Input_T;

typedef struct {
  // TODO add more things we care about for motor controllers
  uint32_t last_updated;
} Mc_Input_T;

typedef struct {
  // TODO add more things we care about for current sensor
  uint16_t bus_voltage;
  uint32_t last_updated;
} Current_Sensor_Input_T;

typedef struct {
  // TODO add more things we care about for shutdown loop
  bool buttons_fault;
  bool bms_fault;
  bool imd_fault;
  bool bpd_fault;
  bool lsc_off;
  bool master_reset;
  bool driver_reset;
  uint8_t lv_voltage;
  uint32_t last_updated;
} Shutdown_Input_T;

typedef struct {
  Can_Node_Input_T *can_node;
  Dash_Input_T *dash;
  Bms_Input_T *bms;
  Mc_Input_T *mc;
  Current_Sensor_Input_T *current_sensor;
  Shutdown_Input_T *shutdown;
  uint32_t msTicks;
} Input_T;

void Input_initialize(Input_T *input);
void Input_update_can(Input_T *input);
void Input_update_pins(Input_T *input);

bool Input_shutdown_loop_closed(Input_T *input);
bool Input_all_devices_alive(Input_T *input);

#endif // _INPUT_H
