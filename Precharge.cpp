#include "Precharge.h"

#define PRECHARGE_PERIOD 7000

void handle_hv_disable(Precharge_State_T *precharge, Pin_Output_T *pin);
void start_precharge(Precharge_State_T *precharge, uint32_t msTicks);
void maybe_end_precharge(Precharge_State_T *precharge, Pin_Output_T *pin, uint32_t msTicks);

void Precharge_update_precharge(Input_T *input, State_T *state, Output_T *output) {
  bool shutdown_loop_updated = input->shutdown->last_updated == input->msTicks;
  if (!shutdown_loop_updated) {
    // Nothing new this round, so can skip it
    return;
  }

  Precharge_State_T *precharge = state->precharge;
  Pin_Output_T *pin = output->pin;

  const bool shutdown_loop_open = !Input_shutdown_loop_closed(input);
  if (shutdown_loop_open) {
    handle_hv_disable(precharge, pin);
    return;
  }

  if (precharge->hv_enabled) {
    // Do nothing
    return;
  }

  const bool precharge_not_started = !precharge->precharge_begun;
  if (precharge_not_started) {
    start_precharge(precharge, input->msTicks);
  } else {
    maybe_end_precharge(precharge, pin, input->msTicks);
  }
}

void handle_hv_disable(Precharge_State_T *precharge, Pin_Output_T *pin) {
  // See if we need to abort anything
  precharge->precharge_begun = false;
  if (precharge->hv_enabled) {
    // Something opened the loop while live
    precharge->hv_enabled = false;
    pin->precharge = Action_OFF;
  }
  return;
}

void start_precharge(Precharge_State_T *precharge, uint32_t msTicks) {
    // Precharge not started, so started
    precharge->precharge_begun = true;
    precharge->last_precharge_start = msTicks;
    return;
}

void maybe_end_precharge(Precharge_State_T *precharge, Pin_Output_T *pin, uint32_t msTicks) {
  // Finally, see if we should end precharge and open the contactor
  const uint32_t end_time = precharge->last_precharge_start + PRECHARGE_PERIOD;
  const bool precharge_ended = msTicks > end_time;
  if (precharge_ended) {
    precharge->precharge_begun = false;
    precharge->hv_enabled = true;
    pin->precharge = Action_ON;
  }
}
