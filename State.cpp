#include "State.h"

#include "Input.h"
#include "Output.h"

#include "Drive.h"
#include "Precharge.h"
#include "Message.h"

void State_update_state(Input_T *input, State_T *state, Output_T *output) {
  Precharge_update_precharge(input, state, output);
  Drive_update_drive(input, state, output);
  Message_update_message(input, state, output);
  // TODO update other
}

void State_initialize(State_T *state) {
  state->precharge->precharge_begun = false;
  state->precharge->last_precharge_start = 0;
  state->precharge->hv_enabled = false;

  state->drive->ready_to_drive = false;
  state->drive->active_aero = false;
  state->drive->limp_mode = false;

  state->message->last_vcu_bms_heartbeat = 0;
  state->message->last_vcu_dash_heartbeat = 0;
  state->message->last_vcu_mc_single_transmit = 0;
  state->message->last_vcu_mc_permanent_transmit = 0;
  state->message->last_vcu_mc_torque = 0;

  state->other->fan_on = false;
  state->other->brake_light = false;
  state->other->vcu_fault = false;
}
