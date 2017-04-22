#ifndef _INPUT_H
#define _INPUT_H

#include <Arduino.h>

#include <MY17_Can_Library.h>

#include "Types.h"

void Input_initialize(Input_T *input);
void Input_fill_input(Input_T *input);

bool Input_shutdown_loop_closed(Input_T *input);
bool Input_all_devices_alive(Input_T *input);

#endif // _INPUT_H
