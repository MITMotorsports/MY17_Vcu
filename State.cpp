#include "State.h"

#include "Input.h"
#include "Output.h"

#include "Drive.h"
#include "Precharge.h"

#define DASH_MSG_PERIOD 100
#define BMS_MSG_PERIOD 1000
#define MC_SINGLE_REQUEST_PERIOD 100
#define MC_TORQUE_CMD_PERIOD 20

void State_update_state(Input_T *input, State_T *state, Output_T *output) {
  Precharge_update_precharge(input, state, output);
  Drive_update_drive(input, state, output);
}

void State_initialize(State_T *state) {

}
