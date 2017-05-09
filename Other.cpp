#include "Other.h"
#include "Input.h"

// Function prototypes
void handle_vcu_fault(Input_T *input, State_T *state, Output_T *output);
void set_vcu_fault(Other_State_T *other, Pin_Output_T *output, bool state);

void Other_update_other(Input_T *input, State_T *state, Output_T *output) {
  handle_vcu_fault(input, state, output);
}

void handle_vcu_fault(Input_T *input, State_T *state, Output_T *output) {
  Other_State_T *other = state->other;
  Pin_Output_T *pin = output->pin;
  Shutdown_Input_T *shutdown = input->shutdown;

  // Heartbeat failures should always cause the vcu to fault
  bool all_devices_alive = Input_all_devices_alive(input);
  if (!all_devices_alive) {
    set_vcu_fault(other, pin, true);
    return;
  }


  // Driver reset releases vcu "latch" if heartbeat check hasn't failed
  if (shutdown->driver_reset) {
    set_vcu_fault(other, pin, false);
    return;
  }

  // Master faults cause vcu to latch, preventing master reset from turning
  // on the car and requiring driver reset to clear this fault.
  // Kevin, this one's for you.
  if (shutdown->bms_fault || shutdown->imd_fault || shutdown->bpd_fault) {
    set_vcu_fault(other, pin, true);
    return;
  }

  // This is true iff
  // (the master switch is open || driver reset has just been pushed),
  // and since the driver reset case is handled above, this indicates tsms.
  // We thus want to open vcu fault to create the "latch".
  bool loop_closed_besides_tsms =
    !shutdown->buttons_fault &&
    !shutdown->bms_fault &&
    !shutdown->imd_fault &&
    !shutdown->bpd_fault &&
    shutdown->lsc_off;

  if (loop_closed_besides_tsms) {
    set_vcu_fault(other, pin, true);
    return;
  }
}

void set_vcu_fault(Other_State_T *other, Pin_Output_T *pin, bool state) {
  if (other->vcu_fault != state) {
    pin->vcu_fault = state ? Action_ON : Action_OFF;
  }
  other->vcu_fault = state;
}
