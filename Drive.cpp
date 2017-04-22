#include "Drive.h"

#include <MY17_Can_Library.h>

void disable_drive(Drive_State_T *drive, Pin_Output_T *pin);
void handle_dash_request(Input_T *input, State_T *state, Output_T *output);
void handle_enable_request(Input_T *input, State_T *state, Output_T *output);
void handle_disable_request(Drive_State_T *drive, Pin_Output_T *pin);

void Drive_update_drive(Input_T *input, State_T *state, Output_T *output)  {
  if (!state->precharge->hv_enabled) {
    // All drive states should go off if hv is off
    disable_drive(state->drive, output->pin);
    return;
  }
  if (input->dash->request_timestamp == input->msTicks) {
    // Got a message from the dash so maybe update state
    handle_dash_request(input, state, output);
  }
}

void disable_drive(Drive_State_T *drive, Pin_Output_T *pin) {
  // HV isn't live, so update accordingly
  if (drive->ready_to_drive) {
    drive->ready_to_drive = false;
    pin->mc_enable = Action_OFF;
  }
  if (drive->active_aero) {
    drive->active_aero = false;
    pin->wing = Action_OFF;
  }
  if (drive->limp_mode) {
    drive->limp_mode = false;
    // TODO change whatever needs to be changed for limp mode
  }
}

void handle_dash_request(Input_T *input, State_T *state, Output_T *output) {
  switch(input->dash->request_type) {
    case CAN_DASH_REQUEST_RTD_ENABLE:
      handle_enable_request(input, state, output);
      break;
    case CAN_DASH_REQUEST_RTD_DISABLE:
      disable_drive(state->drive, output->pin);
      break;
    default:
      // TODO respond to other request types
      break;
  }
}

void handle_enable_request(Input_T *input, State_T *state, Output_T *output) {
  if (state->drive->ready_to_drive) {
    // Already enabled, so do nothing
    return;
  }
  // Note that because we reached this codepath, HV must already be enabled.
  // However, can't hurt to leave it in incase this gets moved around
  bool hv_enabled = state->precharge->hv_enabled;
  bool brake_pressed = input->can_node->brakes_engaged;
  if (hv_enabled && brake_pressed) {
    // Valid enable request, so let's do it!
    state->drive->ready_to_drive = true;
    output->pin->mc_enable = Action_ON;
  }
}
