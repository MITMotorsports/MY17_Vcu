#include "Types.h"

#include <Arduino.h>

#include <MY17_Can_Library.h>

Can_MC_RegID_T Types_MC_Request_to_MC_Reg(MC_Request_Type request_type) {
  switch(request_type) {
    case I_CMD:
      return CAN_MC_REG_CURRENT_CMD;
      break;
    case I_CMD_AFTER_RAMP:
      return CAN_MC_REG_CURRENT_CMD_AFTER_RAMP;
      break;

    case I_ACTUAL:
      return CAN_MC_REG_CURRENT_ACTUAL;
      break;
    case I_ACTUAL_AFTER_DISPLAY:
      return CAN_MC_REG_CURRENT_AFTER_DISPLAY;
      break;

    case I_LIMIT_ACTUAL:
      return CAN_MC_REG_CURRENT_LIMIT_ACTUAL;
      break;

    case N_ACTUAL:
      return CAN_MC_REG_SPEED_ACTUAL_RPM;
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

    case MC_STATE:
      return CAN_MC_REG_STATE;
      break;

    case MC_REQUEST_LENGTH:
    default:
      Serial.println("Should never happen!!");
      return (Can_MC_RegID_T)0;
      break;
  }
}

MC_Request_Type Types_MC_Reg_to_MC_Request(Can_MC_RegID_T reg) {
  switch (reg) {
    case CAN_MC_REG_CURRENT_CMD:
      return I_CMD;
      break;
    case CAN_MC_REG_CURRENT_CMD_AFTER_RAMP:
      return I_CMD_AFTER_RAMP;
      break;

    case CAN_MC_REG_CURRENT_ACTUAL:
      return I_ACTUAL;
      break;
    case CAN_MC_REG_CURRENT_AFTER_DISPLAY:
      return I_ACTUAL_AFTER_DISPLAY;
      break;

    case CAN_MC_REG_CURRENT_LIMIT_ACTUAL:
      return I_LIMIT_ACTUAL;
      break;

    case CAN_MC_REG_SPEED_ACTUAL_RPM:
      return N_ACTUAL;
      break;

    case CAN_MC_REG_MOTOR_TEMP:
      return T_MOTOR;
      break;
    case CAN_MC_REG_IGBT_TEMP:
      return T_IGBT;
      break;
    case CAN_MC_REG_AIR_TEMP:
      return T_AIR;
      break;

    case CAN_MC_REG_STATE:
      return MC_STATE;
      break;

    default:
      // Unused registers are ignored
      return MC_REQUEST_LENGTH;
      break;
  }
}
