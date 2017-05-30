#include "Other.h"
#include "Input.h"

// Function prototypes
void handle_vcu_fault(Input_T *input, State_T *state, Output_T *output);
void set_switch_initialization(Input_T *input, Other_State_T *other);
void handle_brake_light(Input_T *input, State_T *other, Output_T *output);

void set_vcu_fault(Other_State_T *other, Pin_Output_T *output, bool state);

void Other_update_other(Input_T *input, State_T *state, Output_T *output) {
  handle_vcu_fault(input, state, output);
  set_switch_initialization(input, state->other);
  handle_brake_light(input, state, output);
}

void set_switch_initialization(Input_T *input, Other_State_T *other) {
  if (input->shutdown->master_reset) {
    other->master_reset_initialized = true;
  }
  if (input->shutdown->driver_reset) {
    other->driver_reset_initialized = true;
  }
}

void handle_vcu_fault(Input_T *input, State_T *state, Output_T *output) {
  Other_State_T *other = state->other;
  Pin_Output_T *pin = output->pin;
  Shutdown_Input_T *shutdown = input->shutdown;

  // Heartbeat failures should always cause the vcu to fault
  bool all_devices_alive = Input_all_devices_alive(input);
  if (all_devices_alive) {
    other->heartbeat_fault = false;
  } else {
    other->heartbeat_fault = true;
  }

  bool driver_reset_pushed = shutdown->driver_reset;

  /*
    This bool is true iff there is a non-driver-resettable fault.
    We thus want to open vcu fault to create a "latch"
      that is not released unless driver reset is pushed.
    This prevents the master reset button from turning on the car.
  */
  bool has_non_resettable_fault =
    shutdown->bms_fault ||
    shutdown->imd_fault ||
    shutdown->bpd_fault;

  if (driver_reset_pushed) {
    other->master_reset_fault = false;
  } else if (has_non_resettable_fault) {
    other->master_reset_fault = true;
  }

  /*
    This bool is true only if the master switch is in the off position.
    We thus want to open vcu fault to create a "latch"
      that is not released until driver reset is pushed.
    This prevents the master switch from turning on the car.
  */
  bool loop_closed_besides_tsms =
    !has_non_resettable_fault &&
    !shutdown->buttons_fault &&
    shutdown->lsc_off;

  // Order matters here!!
  if (driver_reset_pushed) {
    other->tsms_fault = false;
  } else if (loop_closed_besides_tsms) {
    other->tsms_fault = true;
  }

  bool has_vcu_fault =
    other->heartbeat_fault ||
    other->tsms_fault ||
    other->master_reset_fault;

  set_vcu_fault(other, pin, has_vcu_fault);
}

void handle_brake_light(Input_T *input, State_T *state, Output_T *output) {
  if (input->front_can_node->brakes_engaged && !state->other->brake_light) {
    state->other->brake_light = true;
    output->pin->brake_light = Action_ON;
  } else if (!input->front_can_node->brakes_engaged && state->other->brake_light) {
    state->other->brake_light = false;
    output->pin->brake_light = Action_OFF;
  }
}

void set_vcu_fault(Other_State_T *other, Pin_Output_T *pin, bool state) {
  if (other->vcu_fault != state) {
    pin->vcu_fault = state ? Action_ON : Action_OFF;
  }
  other->vcu_fault = state;
}
