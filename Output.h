#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <Arduino.h>

#include <MY17_Can_Library.h>

typedef enum {
  Action_NONE = 0,
  Action_ON = 1,
  Action_OFF = 2
} Pin_Action_T;

typedef struct {
  bool send_dash_msg;
  bool send_bms_msg;
  bool send_mc_single_request_msg;
  bool send_mc_permanent_request_msg;
  bool send_torque_cmd;
} Can_Output_T;

typedef struct {
  Pin_Action_T fan;
  Pin_Action_T brake;
  Pin_Action_T precharge;
  Pin_Action_T mc_enable;
  Pin_Action_T wing;
  Pin_Action_T vcu_fault;
} Pin_Output_T;

typedef struct {
  // TODO
  bool temp;
} Onboard_Output_T;

typedef struct {
  // TODO
  bool temp;
} Xbee_Output_T;

typedef struct {
  Can_Output_T *can;
  Pin_Output_T *pin;
  Onboard_Output_T *onboard;
  Xbee_Output_T *xbee;
} Output_T;

#endif // _OUTPUT_H

