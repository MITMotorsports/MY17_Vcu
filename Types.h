#ifndef _TYPES_H_
#define _TYPES_H_

#include <Arduino.h>

#include <MY17_Can_Library.h>

// All names in the enum are taken from the NDrive manual
typedef enum {
  I_CMD,
  I_ACTUAL,
  I_CMD_RAMP,
  I_Q_ACTUAL,
  I_D_ACTUAL,

  N_CMD,
  N_ACTUAL,
  N_CMD_RAMP,

  DC_BUS,
  V_OUT,
  V_RED,
  V_Q,
  V_D,

  T_MOTOR,
  T_IGBT,
  T_AIR,

  P_MOTOR,

  P_REGEN_I2_T,

  NO_MOTOR_CONTROLLER_REQUEST
} MC_Request_Type;

typedef struct {
  int16_t requested_torque;
  uint8_t brake_pressure;
  bool brakes_engaged;
  uint32_t last_updated;
} Front_Can_Node_Input_T;

typedef struct {
  uint32_t last_updated;
} Rear_Can_Node_Input_T;

typedef struct {
  Can_Dash_RequestID_T request_type;
  uint32_t request_timestamp;
  uint32_t last_updated;
} Dash_Input_T;

typedef struct {
  // TODO handle bms state of charge, either here or in can current sensor
  Can_Bms_StateID_T state;
  uint32_t last_updated;
} Bms_Input_T;

typedef struct {
  // TODO add any errors if necessary for motor controllers
  uint32_t last_i_cmd;
  uint32_t last_i_actual;
  uint32_t last_i_cmd_ramp;
  uint32_t last_i_q_actual;
  uint32_t last_i_d_actual;

  uint32_t last_n_cmd;
  uint32_t last_n_actual;
  uint32_t last_n_cmd_ramp;

  uint32_t last_dc_bus;
  uint32_t last_v_out;
  uint32_t last_v_red;
  uint32_t last_v_q;
  uint32_t last_v_d;

  uint32_t last_t_motor;
  uint32_t last_t_igbt;
  uint32_t last_t_air;

  uint32_t last_p_motor;

  uint32_t last_p_regen_i2_t;

  uint32_t last_updated;
} Mc_Input_T;

typedef struct {
  int32_t voltage_mV;
  uint32_t last_voltage_ms;
  int32_t current_mA;
  uint32_t last_current_ms;
  int32_t power_W;
  uint32_t last_power_ms;
  int32_t energy_Wh;
  uint32_t last_energy_ms;
  // Used for heartbeat
  uint32_t last_updated;
} Current_Sensor_Input_T;

typedef struct {
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
  Front_Can_Node_Input_T *front_can_node;
  Rear_Can_Node_Input_T *rear_can_node;
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
  uint32_t last_vcu_bms_heartbeat_ms;
  uint32_t last_vcu_dash_heartbeat_ms;
  uint32_t last_vcu_mc_single_transmit_ms;
  uint32_t last_vcu_mc_permanent_transmit_ms;
  uint32_t last_vcu_mc_torque_ms;
  uint32_t last_front_can_log_ms;
} Message_State_T;

typedef struct {
  MC_Request_Type permanent_transmit_request_type;
  uint32_t last_permanent_transmit_request_ms;
} Mc_Request_State_T;

typedef struct {
  bool fan_on;
  bool brake_light;
  bool heartbeat_fault;
  bool tsms_fault;
  bool master_reset_fault;
  bool vcu_fault;
  bool master_reset_initialized;
  bool driver_reset_initialized;
} Other_State_T;

typedef struct {
  Precharge_State_T *precharge;
  Drive_State_T *drive;
  Message_State_T *message;
  Mc_Request_State_T *mc_request_state;
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
  bool write_current_log;
  bool write_voltage_log;
  bool write_power_log;
  bool write_energy_log;
  bool write_front_can_log;
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
