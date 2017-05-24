#include "State.h"

#include "Input.h"
#include "Output.h"

#include "Drive.h"
#include "Precharge.h"
#include "Message.h"
#include "Other.h"

void State_update_state(Input_T *input, State_T *state, Output_T *output) {
  Precharge_update_precharge(input, state, output);
  Drive_update_drive(input, state, output);
  Message_update_message(input, state, output);
  Other_update_other(input, state, output);
}

void State_initialize(State_T *state) {
  state->precharge->precharge_begun = false;
  state->precharge->last_precharge_start = 0;
  state->precharge->hv_enabled = false;

  state->drive->ready_to_drive = false;
  state->drive->active_aero = false;
  state->drive->limp_mode = false;

  state->message->last_vcu_bms_heartbeat_ms = 0;
  state->message->last_vcu_dash_heartbeat_ms = 0;
  state->message->last_vcu_mc_single_transmit_ms = 0;
  state->message->last_vcu_mc_permanent_transmit_ms = 0;
  state->message->last_vcu_mc_torque_ms = 0;
  state->message->last_front_can_log_ms = 0;

  state->other->fan_on = false;
  state->other->brake_light = false;
  state->other->heartbeat_fault = false;
  state->other->tsms_fault = false;
  state->other->master_reset_fault = false;
  state->other->vcu_fault = false;
  state->other->master_reset_initialized = false;
  state->other->driver_reset_initialized = false;
}
