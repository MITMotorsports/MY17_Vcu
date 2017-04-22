#ifndef _TYPES_H_
#define _TYPES_H_

#include <Arduino.h>

#include <MY17_Can_Library.h>

typedef struct {
  // TODO add more things we care about for CAN Node
  int16_t requested_torque;
  bool brakes_engaged;
  uint32_t last_updated;
} Can_Node_Input_T;

typedef struct {
  // TODO add more things we care about for dash
  Can_Dash_RequestID_T request_type;
  uint32_t request_timestamp;
  uint32_t last_updated;
} Dash_Input_T;

typedef struct {
  // TODO add more things we care about for BMS
  Can_Bms_StateID_T state;
  uint32_t last_updated;
} Bms_Input_T;

typedef struct {
  // TODO add more things we care about for motor controllers
  uint32_t last_updated;
} Mc_Input_T;

typedef struct {
  // TODO add more things we care about for current sensor
  uint16_t bus_voltage;
  uint32_t last_updated;
} Current_Sensor_Input_T;

typedef struct {
  // TODO add more things we care about for shutdown loop
  bool buttons_fault;
  bool bms_fault;
  bool imd_fault;
  bool bpd_fault;
  bool lsc_off;
  bool master_reset;
  bool driver_reset;
  uint8_t lv_voltage;
  uint32_t last_updated;
} Shutdown_Input_T;

typedef struct {
  Can_Node_Input_T *can_node;
  Dash_Input_T *dash;
  Bms_Input_T *bms;
  Mc_Input_T *mc;
  Current_Sensor_Input_T *current_sensor;
  Shutdown_Input_T *shutdown;
  uint32_t msTicks;
} Input_T;

typedef struct {
  bool precharge_begun;
  uint32_t last_precharge_start;
  bool hv_enabled;
} Precharge_State_T;

typedef struct {
  bool ready_to_drive;
  bool active_aero;
  bool limp_mode;
} Drive_State_T;

typedef struct {
  uint32_t last_vcu_bms_heartbeat;
  uint32_t last_vcu_dash_heartbeat;
  uint32_t last_vcu_mc_single_transmit;
  uint32_t last_vcu_mc_permanent_transmit;
  uint32_t last_vcu_mc_torque;
} Message_State_T;

typedef struct {
  bool fan_on;
  bool brake_light;
  bool vcu_fault;
} Other_State_T;

typedef struct {
  Precharge_State_T *precharge;
  Drive_State_T *drive;
  Message_State_T *message;
  Other_State_T *other;
} State_T;


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
  Pin_Action_T brake_light;
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

#endif // _TYPES_H_
