#ifndef _STATE_H_
#define _STATE_H_

#include <Arduino.h>

#include "Input.h"
#include "Output.h"

typedef struct {
  bool precharge_begun;
  uint32_t last_precharge_start;
  bool hv_enabled;
} Precharge_State_T;

typedef struct {
  bool ready_to_drive;
  bool active_aero;
  bool limp_mode;
} Drive_State_T;

typedef struct {
  uint32_t last_vcu_bms_heartbeat;
  uint32_t last_vcu_dash_heartbeat;
  uint32_t last_vcu_mc_single_transmit;
  uint32_t last_vcu_mc_permanent_transmit;
  uint32_t last_vcu_mc_torque;
} Can_Timing_State_T;

typedef struct {
  bool fan_on;
  bool brake_light;
  bool vcu_fault;
} Other_State_T;

typedef struct {
  Precharge_State_T *precharge;
  Drive_State_T *drive;
  Can_Timing_State_T *can_timing;
  Other_State_T *other;
} State_T;

void State_update_state(Input_T *input, State_T *state, Output_T *output);
#endif // _STATE_H_
