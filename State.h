#ifndef _STATE_H
#define _STATE_H

#include <stdint.h>

struct Pedal_State {
  int16_t requested_torque;
  bool brakes_engaged;
}

struct Shutdown_State {
  bool buttons_fault;
  bool bms_fault;
  bool imd_fault;
  bool bspd_fault;
  bool vcu_fault;
  bool lsc_low;
  uint32_t 
}

#endif // _STATE_H
