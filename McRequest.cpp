#include "McRequest.h"

#include "Types.h"

#include "MY17_Can_Library.h"

void update_permanent_transmit_request_type(Input_T *input, State_T *state);

void McRequest_update_mcRequest(Input_T *input, State_T *state, Output_T *output) {
  update_permanent_transmit_request_type(input, state);

  const bool notFinishedSendingRequests = 
    state->mc_request_state->permanent_transmit_request_type != NO_MOTOR_CONTROLLER_REQUEST;
  const uint32_t lastRequest_ms = state->mc_request_state->last_permanent_transmit_request_ms;
  const uint32_t msTicks = input->msTicks;
  const uint32_t nextPermanentTransmitRequest_ms = lastRequest_ms + msTicks;
  if ((nextPermanentTransmitRequest_ms < msTicks) && notFinishedSendingRequests) {
    state->mc_request_state->last_permanent_transmit_request_ms = msTicks;
    output->can->send_mc_permanent_request_msg = true;
  }
}

Can_MC_RegID_T McRequest_translate_mc_request(MC_Request_Type requestType) {
  switch(requestType) {
    case I_CMD:
      return CAN_MC_REG_CURRENT_CMD;
      break;
    case I_ACTUAL:
      return CAN_MC_REG_CURRENT_ACTUAL;
      break;
    case I_CMD_RAMP:
      return CAN_MC_REG_CURRENT_CMD_AFTER_RAMP;
      break;
    case I_Q_ACTUAL:
      return CAN_MC_REG_CURRENT_ACTUAL_I_Q;
      break;
    case I_D_ACTUAL:
      return CAN_MC_REG_CURRENT_ACTUAL_I_D;
      break;

    case N_CMD:
      return CAN_MC_REG_SPEED_CMD_BEFORE_RAMP_RPM;
      break;
    case N_ACTUAL:
      return CAN_MC_REG_SPEED_ACTUAL_RPM;
      break;
    case N_CMD_RAMP:
      return CAN_MC_REG_SPEED_CMD_AFTER_RAMP_RPM;
      break;

    case DC_BUS:
      return CAN_MC_REG_BUS_VOLTAGE;
      break;
    case V_OUT:
      return CAN_MC_REG_OUTPUT_VOLTAGE;
      break;
    case V_RED:
      return CAN_MC_REG_VOLTAGE_LIMITATION;
      break;
    case V_Q:
      return CAN_MC_REG_VOLTAGE_V_Q;
      break;
    case V_D:
      return CAN_MC_REG_VOLTAGE_V_D;
      break;

    case T_MOTOR:
      return CAN_MC_REG_MOTOR_TEMP;
      break;
    case T_IGBT:
      return CAN_MC_REG_IGBT_TEMP;
      break;
    case T_AIR:
      return CAN_MC_REG_AIR_TEMP;
      break;

    case P_MOTOR:
      return CAN_MC_REG_MOTOR_POWER;
      break;

    case P_REGEN_I2_T:
      return CAN_MC_REG_BALLAST_POWER_CAPACITY_I2XT;
      break;

    default:
      // TODO: Not ideal to return current_cmd by default.
      return CAN_MC_REG_CURRENT_CMD;
      break;
  }
}

void update_permanent_transmit_request_type(Input_T *input, State_T *state) {
  MC_Request_Type requestType;

  if (state->mc_request_state->permanent_transmit_request_type == 
      NO_MOTOR_CONTROLLER_REQUEST) {
    requestType = NO_MOTOR_CONTROLLER_REQUEST;
  }

  else if (input->mc->last_i_cmd == 0) {
    requestType = I_CMD;
  } else if (input->mc->last_i_actual == 0) {
    requestType = I_ACTUAL;
  } else if (input->mc->last_i_cmd_ramp == 0) {
    requestType = I_CMD_RAMP;
  } else if (input->mc->last_i_q_actual == 0) {
    requestType = I_Q_ACTUAL;
  } else if (input->mc->last_i_d_actual == 0) {
    requestType = I_D_ACTUAL;
  }

  else if (input->mc->last_n_cmd == 0) {
    requestType = N_CMD;
  } else if (input->mc->last_n_actual == 0) {
    requestType = N_ACTUAL;
  } else if (input->mc->last_n_cmd_ramp == 0) {
    requestType = N_CMD_RAMP;
  }

  else if (input->mc->last_dc_bus == 0) {
    requestType = DC_BUS;
  } else if (input->mc->last_v_out == 0) {
    requestType = V_OUT;
  } else if (input->mc->last_v_red == 0) {
    requestType = V_RED;
  } else if (input->mc->last_v_q == 0) {
    requestType = V_Q;
  } else if (input->mc->last_v_d == 0) {
    requestType = V_D;
  }

  else if (input->mc->last_t_motor == 0) {
    requestType = T_MOTOR;
  } else if (input->mc->last_t_igbt == 0) {
    requestType = T_IGBT;
  } else if (input->mc->last_t_air == 0) {
    requestType = T_AIR;
  }

  else if (input->mc->last_p_motor == 0) {
    requestType = P_MOTOR;
  }

  else if (input->mc->last_p_regen_i2_t == 0) {
    requestType = P_REGEN_I2_T;
  }

  else {
    requestType = NO_MOTOR_CONTROLLER_REQUEST;
  }

  state->mc_request_state->permanent_transmit_request_type = requestType;
}
