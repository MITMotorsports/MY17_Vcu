#include "Drive.h"

#include <MY17_Can_Library.h>

void disable_drive(Drive_State_T *drive, Pin_Output_T *pin);
void handle_dash_request(Input_T *input, State_T *state, Output_T *output);
void handle_enable_request(Input_T *input, State_T *state, Output_T *output);
void handle_disable_request(Drive_State_T *drive, Pin_Output_T *pin);
void handle_active_aero_request(Drive_State_T *drive, Pin_Output_T *pin, bool state);
void handle_limp_mode_request(Drive_State_T *drive, bool state);
void handle_data_flag_request(uint32_t msTicks);

Can_Vcu_LimpState_T next_limp_state(Can_Vcu_LimpState_T limp_state);

void Drive_update_drive(Input_T *input, State_T *state, Output_T *output)  {
  if (!state->precharge->hv_enabled) {
    // All drive states should go off if hv is off
    disable_drive(state->drive, output->pin);
    // Don't want to even process RTD requests if HV off
    Can_Dash_RequestID_T type = input->dash->request_type;
    if (type == CAN_DASH_REQUEST_RTD_ENABLE || type == CAN_DASH_REQUEST_RTD_DISABLE) {
      return;
    }
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
    // TODO remove this once fan logic better
    pin->fan = Action_OFF;
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
    case CAN_DASH_REQUEST_ACTIVE_AERO_ENABLE:
      Serial.println("AERO ENGAGE");
      handle_active_aero_request(state->drive, output->pin, true);
      break;
    case CAN_DASH_REQUEST_ACTIVE_AERO_DISABLE:
      Serial.println("AERO RELEASE");
      handle_active_aero_request(state->drive, output->pin, false);
      break;
    case CAN_DASH_REQUEST_LIMP_MODE_DISABLE:
      handle_limp_mode_request(state->drive, false);
      break;
    case CAN_DASH_REQUEST_LIMP_MODE_ENABLE:
      handle_limp_mode_request(state->drive, true);
      break;
    case CAN_DASH_REQUEST_DATA_FLAG:
      handle_data_flag_request(input->msTicks);
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
  bool brake_pressed = input->front_can_node->brakes_engaged;
  if (hv_enabled && brake_pressed) {
    // Valid enable request, so let's do it!
    state->drive->ready_to_drive = true;
    output->pin->mc_enable = Action_ON;
    // TODO remove this once fan logic better
    output->pin->fan = Action_ON;
  }
}

void handle_active_aero_request(Drive_State_T *drive, Pin_Output_T *pin, bool state) {
  if (state != drive->active_aero) {
    drive->active_aero = state;
    pin->wing = state ? Action_ON : Action_OFF;
  }
}

void handle_limp_mode_request(Drive_State_T *drive, bool state) {
  if (!state) {
    drive->limp_mode = CAN_LIMP_NORMAL;
  } else {
    drive->limp_mode = next_limp_state(drive->limp_mode);
  }
}

Can_Vcu_LimpState_T next_limp_state(Can_Vcu_LimpState_T limp_state) {
  switch(limp_state) {
    case CAN_LIMP_NORMAL:
      return CAN_LIMP_50;
      break;
    case CAN_LIMP_50:
      return CAN_LIMP_33;
      break;
    case CAN_LIMP_33:
      return CAN_LIMP_25;
      break;
    case CAN_LIMP_25:
    default:
      return CAN_LIMP_NORMAL;
      break;
  }
}

void handle_data_flag_request(uint32_t msTicks) {
  String line;
  line.concat("DATA_FLAG,1,");
  line.concat(msTicks);
  Serial.println(line);
  Serial1.println(line);
  Serial2.println(line);
}

