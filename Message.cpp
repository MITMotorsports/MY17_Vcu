#include "Message.h"

#define DASH_MSG_PERIOD 100
#define BMS_MSG_PERIOD 1000
#define MC_SINGLE_REQUEST_PERIOD 100
#define MC_TORQUE_CMD_PERIOD 20

void update_can(Input_T *input, State_T *state, Output_T *output);

void Message_update_message(Input_T *input, State_T *state, Output_T *output) {
  update_can(input, state, output);
}

void update_can(Input_T *input, State_T *state, Output_T *output) {
  Message_State_T *message = state->message;
  const uint32_t msTicks = input->msTicks;

  const uint32_t last_bms = message->last_vcu_bms_heartbeat;
  const uint32_t next_bms = last_bms + BMS_MSG_PERIOD;
  if (last_bms == 0 || next_bms < msTicks) {
    message->last_vcu_bms_heartbeat = msTicks;
    output->can->send_bms_msg = true;
  }

  const uint32_t last_dash = message->last_vcu_dash_heartbeat;
  const uint32_t next_dash = last_dash + DASH_MSG_PERIOD;
  if (last_dash == 0 || next_dash < msTicks) {
    message->last_vcu_dash_heartbeat = msTicks;
    output->can->send_dash_msg = true;
  }
}

