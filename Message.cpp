#include "Message.h"

#define DASH_MSG_PERIOD 100UL
#define BMS_MSG_PERIOD 1000UL
#define MC_SINGLE_REQUEST_PERIOD 100UL
#define MC_PERMANENT_TRANSMIT_PERIOD 100UL
#define MC_TORQUE_CMD_PERIOD 20UL

#define FRONT_CAN_LOG_PERIOD_MS 50UL

void update_can(Input_T *input, State_T *state, Output_T *output);
void update_onboard(Input_T *input, State_T *state, Output_T *output);

void update_can_bms_heartbeat(State_T *state, Output_T *output, uint32_t msTicks);
void update_can_dash_heartbeat(State_T *state, Output_T *output, uint32_t msTicks);
void update_can_mc_torque(State_T *state, Output_T *output, uint32_t msTicks);
void update_can_mc_request(Input_T *input, State_T *state, Output_T *output);

void update_onboard_front_can(State_T *state, Output_T *output, uint32_t msTicks);
void update_onboard_current_sense(Input_T *input, State_T *state, Output_T *output);
void update_onboard_mc_response(Input_T *input, State_T *state, Output_T *output);

bool period_reached(uint32_t start, uint32_t period, uint32_t msTicks);

void Message_update_message(Input_T *input, State_T *state, Output_T *output) {
  update_can(input, state, output);
  update_onboard(input, state, output);
}

void update_can(Input_T *input, State_T *state, Output_T *output) {
  const uint32_t msTicks = input->msTicks;

  update_can_bms_heartbeat(state, output, msTicks);
  update_can_dash_heartbeat(state, output, msTicks);
  update_can_mc_torque(state, output, msTicks);
  update_can_mc_request(input, state, output);
}

void update_onboard(Input_T *input, State_T *state, Output_T *output) {
  update_onboard_front_can(state, output, input->msTicks);
  update_onboard_current_sense(input, state, output);
}

void update_can_bms_heartbeat(State_T *state, Output_T *output, uint32_t msTicks) {
  uint32_t *last_msg = &state->message->last_vcu_bms_heartbeat_ms;

  if(period_reached(*last_msg, BMS_MSG_PERIOD, msTicks)) {
    *last_msg = msTicks;
    output->can->send_bms_msg = true;
  }
}

void update_can_dash_heartbeat(State_T *state, Output_T *output, uint32_t msTicks) {
  uint32_t *last_msg = &state->message->last_vcu_dash_heartbeat_ms;

  if(period_reached(*last_msg, DASH_MSG_PERIOD, msTicks)) {
    *last_msg = msTicks;
    output->can->send_dash_msg = true;
  }

}

void update_can_mc_torque(State_T *state, Output_T *output, uint32_t msTicks) {
  // Only send torque commands if ready to drive!
  if (!state->drive->ready_to_drive) {
    return;
  }

  uint32_t *last_msg = &state->message->last_vcu_mc_torque_ms;

  if(period_reached(*last_msg, MC_TORQUE_CMD_PERIOD, msTicks)) {
    *last_msg = msTicks;
    output->can->send_torque_cmd = true;
  }
}

void update_can_mc_request(Input_T *input, State_T *state, Output_T *output) {
  const uint32_t msTicks = input->msTicks;
  uint32_t *last_msg = &state->message->last_vcu_mc_permanent_transmit_ms;

  if (period_reached(*last_msg, MC_PERMANENT_TRANSMIT_PERIOD, msTicks)) {
    // We want to find the first message type that hasn't yet responded to
    // its request.
    for (int i = 0; i < MC_REQUEST_LENGTH; i++) {
      if (input->mc->last_mc_response_times[i] == 0) {
        // This is the next message type we want to request transmit of.
        *last_msg = msTicks;
        output->can->send_mc_permanent_request_msg[i] = true;
        return;
      }
    }
    // All message types have responded, so we can just chill.
    *last_msg = UINT32_MAX;
  }
}

void update_onboard_front_can(State_T *state, Output_T *output, uint32_t msTicks) {
  uint32_t *last_msg = &state->message->last_front_can_log_ms;

  if(period_reached(*last_msg, FRONT_CAN_LOG_PERIOD_MS, msTicks)) {
    *last_msg = msTicks;
    output->onboard->write_front_can_log = true;
  }
}

void update_onboard_current_sense(Input_T *input, State_T *state, Output_T *output) {
  const uint32_t msTicks = input->msTicks;

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

void update_onboard_mc_response(Input_T *input, State_T *state, Output_T *output) {
  MC_Request_Type type = Types_MC_Request_get(input->mc);
  if (type != MC_REQUEST_LENGTH) {
      output->onboard->write_mc_data[type] = true;
  }
}

bool period_reached(uint32_t start, uint32_t period, uint32_t msTicks) {
  const uint32_t next_time = start + period;
  return next_time < msTicks;
}
