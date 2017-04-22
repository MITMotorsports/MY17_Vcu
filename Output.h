#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <Arduino.h>

#include <MY17_Can_Library.h>

#include "Types.h"

void Output_initialize(Output_T *output);
void Output_empty_output(Input_T *input, State_T *state, Output_T *output);

#endif // _OUTPUT_H

