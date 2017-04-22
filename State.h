#ifndef _STATE_H_
#define _STATE_H_

#include <Arduino.h>

#include "Types.h"

void State_initialize(State_T *state);
void State_update_state(Input_T *input, State_T *state, Output_T *output);

#endif // _STATE_H_
