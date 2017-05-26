#include "Message.h"

#define DASH_MSG_PERIOD 100UL
#define BMS_MSG_PERIOD 1000UL
#define MC_SINGLE_REQUEST_PERIOD 100UL
#define MC_PERMANENT_TRANSMIT_PERIOD 100UL
#define MC_TORQUE_CMD_PERIOD 20UL

#define FRONT_CAN_LOG_PERIOD_MS 50UL

void update_can(Input_T *input, State_T *state, Output_T *output);
void update_onboard(Input_T *input, State_T *state, Output_T *output);
void maybe_update(uint32_t *last_time, uint32_t period, bool *output_flag, uint32_t msTicks);

void Message_update_message(Input_T *input, State_T *state, Output_T *output) {
  update_can(input, state, output);
  update_onboard(input, state, output);
}

void update_can(Input_T *input, State_T *state, Output_T *output) {
  Message_State_T *message = state->message;
  Can_Output_T *can = output->can;
  const uint32_t msTicks = input->msTicks;

  maybe_update(
      &message->last_vcu_bms_heartbeat_ms,
      BMS_MSG_PERIOD,
      &can->send_bms_msg,
      msTicks);

  maybe_update(
      &message->last_vcu_dash_heartbeat_ms,
      DASH_MSG_PERIOD,
      &can->send_dash_msg,
      msTicks);

  if (state->drive->ready_to_drive) {
    // Currently we don't zero the motors when disabled but let the MC disable
    // pin do it automatically. Hopefully this isn't a problem later on...
    maybe_update(
        &message->last_vcu_mc_torque_ms,
        MC_TORQUE_CMD_PERIOD,
        &can->send_torque_cmd,
        msTicks);
  }
}

void update_onboard(Input_T *input, State_T *state, Output_T *output) {
  const uint32_t msTicks = input->msTicks;

  // Front Can Node data comes in too fast to log as it comes,
  // so we have to do periodic logging.
  maybe_update(
      &state->message->last_front_can_log_ms,
      FRONT_CAN_LOG_PERIOD_MS,
      &output->onboard->write_front_can_log,
      msTicks);

  if (state->drive->ready_to_drive) {
    // For now we just log all current sensor readings as they come in.
    // TODO consider changing this if we fill up serial baud rate.
    Current_Sensor_Input_T *sensor = input->current_sensor;
    Onboard_Output_T *onboard = output->onboard;

    if (sensor->last_current_ms == msTicks) {
      onboard->write_current_log = true;
    }
    if (sensor->last_voltage_ms == msTicks) {
      onboard->write_voltage_log = true;
    }
    if (sensor->last_power_ms == msTicks) {
      onboard->write_power_log = true;
    }
    if (sensor->last_energy_ms == msTicks) {
      onboard->write_energy_log = true;
    }
  }
}

void maybe_update(uint32_t *last_time, uint32_t period, bool *output_flag, uint32_t msTicks) {
  const uint32_t next_time = *last_time + period;
  if (next_time < msTicks) {
    *last_time = msTicks;
    *output_flag = true;
  }
}
