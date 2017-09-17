#include "Output.h"

#include <Arduino.h>

#include "Input.h"
#include "Pins.h"

#include "MY17_Can_Library.h"

#define DISABLE_LOG_PERIOD_MS 100UL
static uint32_t last_disable_log_time_ms = 0;

static uint32_t last_log_time_ms = 0;

void handle_can(Input_T *input, State_T *state, Can_Output_T *can);
void handle_pins(Pin_Output_T *output);
void handle_onboard(Input_T *input, State_T *state, Onboard_Output_T *onboard);

void send_dash_msg(Input_T *input, State_T *state);
void send_bms_msg(Input_T *input, State_T *state);
void send_torque_cmd_msg(Input_T *input, State_T *state);
void send_mc_request_msg(MC_Request_Type requestType);

void get_mc_name(MC_Request_Type type, String& output);
void print_data(String prefix, int32_t data, uint32_t msTicks);

void Output_initialize(Output_T *output) {
  output->can->send_dash_msg = false;
  output->can->send_bms_msg = false;
  output->can->send_torque_cmd = false;
  for (int i = 0; i < MC_REQUEST_LENGTH; i++) {
    output->can->send_mc_permanent_request_msg[i] = false;
  }

  output->pin->fan = Action_OFF;
  output->pin->brake_light = Action_OFF;
  output->pin->precharge = Action_OFF;
  output->pin->mc_enable = Action_OFF;
  output->pin->wing = Action_OFF;
  output->pin->vcu_fault = Action_OFF;

  output->onboard->write_current_log = false;
  output->onboard->write_voltage_log = false;
  output->onboard->write_power_log = false;
  output->onboard->write_energy_log = false;
  output->onboard->write_front_can_log = false;
  output->onboard->write_fault_log = false;
  for (int i = 0; i < MC_REQUEST_LENGTH; i++) {
    output->onboard->write_mc_data[i] = false;
  }
  output->onboard->write_speed_log = false;

  // TODO
  output->xbee->temp = false;
}

void Output_empty_output(Input_T *input, State_T *state, Output_T *output) {
  handle_can(input, state, output->can);
  handle_pins(output->pin);
  handle_onboard(input, state, output->onboard);
}

void handle_onboard(Input_T *input, State_T *state, Onboard_Output_T *onboard) {
  // Don't bother logging if not driving
  bool ready_to_drive = state->drive->ready_to_drive;

  if (!ready_to_drive) {
    if (last_disable_log_time_ms + DISABLE_LOG_PERIOD_MS < input->msTicks) {
      last_disable_log_time_ms = input->msTicks;
      print_data("DRIVE_DISABLED", 1, last_disable_log_time_ms);
    }
    return;
  }

  // We are driving, so log as normal
  Current_Sensor_Input_T *sensor = input->current_sensor;

  // TODO HACK log current time instead of reading time
  const uint32_t curr_time = input->msTicks;

  if (onboard->write_current_log) {
    onboard->write_current_log = false;
    print_data("crt", sensor->current_mA, curr_time); //mA
  }
  if (onboard->write_voltage_log) {
    onboard->write_voltage_log = false;
    print_data("vlt", sensor->voltage_mV, curr_time); //mV
  }
  if (onboard->write_power_log) {
    onboard->write_power_log = false;
    print_data("pwr", sensor->power_W, curr_time); //W
  }

  if (onboard->write_speed_log) {
    onboard->write_speed_log = false;
    print_data("wfl", input->speed->rpm[FL_WHEEL], input->speed->last_updated[FL_WHEEL]);
    print_data("wfr", input->speed->rpm[FR_WHEEL], input->speed->last_updated[FR_WHEEL]);
    print_data("wrl", input->speed->rpm[RL_WHEEL], input->speed->last_updated[RL_WHEEL]);
    print_data("wrr", input->speed->rpm[RR_WHEEL], input->speed->last_updated[RR_WHEEL]);
  }

  if (onboard->write_front_can_log) {
    Front_Can_Node_Input_T *front_can = input->front_can_node;
    // TODO HACK log current time instead of reading time
    const uint32_t curr_time = input->msTicks;

    onboard->write_front_can_log = false;
    print_data("trq", front_can->requested_torque, curr_time); //int16_t
    print_data("brk", front_can->brake_pressure, curr_time); //uint8_t
  }

  if (onboard->write_fault_log) {
    onboard->write_fault_log = false;

    Bms_Input_T *bms = input->bms;
    const uint32_t curr_time = input->msTicks;

    print_data("tmp", bms->highest_cell_temp_dC, curr_time); //dC
    print_data("lcv", bms->lowest_cell_voltage_cV, curr_time); //cV
  }

  for (int i = 0; i < MC_REQUEST_LENGTH; i++) {
    Mc_Input_T *mc = input->mc;
    const uint32_t curr_time = input->msTicks;
    if (onboard->write_mc_data[i]) {
      String name;
      if (i != MC_STATE) {
        get_mc_name((MC_Request_Type)i, name);
        print_data(name, mc->data[i], curr_time); //int16_t
      } else {
        if (mc->state.current_limit_to_continuous) {
          print_data("s05", 1, curr_time);
        }
        if (mc->state.speed_limited_via_switch) {
          print_data("s17", 1, curr_time);
        }
        if (mc->state.current_limited_via_switch) {
          print_data("s20", 1, curr_time);
        }
        if (mc->state.active_current_reduction) {
          print_data("s21", 1, curr_time);
        }
        if (mc->state.current_limited_via_speed) {
          print_data("s22", 1, curr_time);
        }
        if (mc->state.current_limited_via_igbt_temp) {
          print_data("s23", 1, curr_time);
        }
        if (mc->state.current_limited_to_continuous_via_igbt_temp) {
          print_data("s24", 1, curr_time);
        }
        // Low frequency bit is true if no current reduction
        if (!mc->state.current_reduction_low_frequency) {
          print_data("s25", 1, curr_time);
        }
        if (mc->state.current_reduction_via_motor_temp) {
          print_data("s26", 1, curr_time);
        }
        if (mc->state.current_reduction_via_analog_input) {
          print_data("s27", 1, curr_time);
        }
        if (mc->state.current_accumulator_limit_charged) {
          print_data("s28", 1, curr_time);
        }
      }
      onboard->write_mc_data[i] = false;
    }
  }
}

void print_data(String prefix, int32_t data, uint32_t msTicks) {
  // TODO investigate using diff time
  uint32_t diff = msTicks - last_log_time_ms;
  last_log_time_ms = msTicks;
  String line;
  line.concat(prefix);
  line.concat(",");
  line.concat(data);
  line.concat(",");
  line.concat(msTicks);
  Serial1.println(line);
}

void get_mc_name(MC_Request_Type type, String& output) {
  switch(type) {
    case I_CMD:
      output.concat("x26");
      break;
    case I_CMD_AFTER_RAMP:
      output.concat("x22");
      break;
    case I_ACTUAL:
      output.concat("x20");
      break;
    case I_ACTUAL_AFTER_DISPLAY:
      output.concat("x5F");
      break;
    case I_LIMIT_ACTUAL:
      output.concat("x48");
      break;
    case N_ACTUAL:
      output.concat("x30");
      break;
    case T_MOTOR:
      output.concat("x49");
      break;
    case T_IGBT:
      output.concat("x4A");
      break;
    case T_AIR:
      output.concat("x4B");
      break;
    case MC_REQUEST_LENGTH:
    case MC_STATE:
      output.concat("BUG!BUG!");
      break;
  }
}

void handle_can(Input_T *input, State_T *state, Can_Output_T *can) {
  if (can->send_dash_msg) {
    can->send_dash_msg = false;
    send_dash_msg(input, state);
  }

  if (can->send_bms_msg) {
    can->send_bms_msg = false;
    send_bms_msg(input, state);
  }

  if (can->send_torque_cmd) {
    can->send_torque_cmd = false;
    send_torque_cmd_msg(input, state);
  }

  for (int i = 0; i < MC_REQUEST_LENGTH; i++) {
    if (can->send_mc_permanent_request_msg[i]) {
      can->send_mc_permanent_request_msg[i] = false;
      MC_Request_Type type = (MC_Request_Type)i;
      send_mc_request_msg(type);
    }
  }
}

void send_dash_msg(Input_T *input, State_T *state) {
  Can_Vcu_DashHeartbeat_T msg;
  msg.rtd_light = state->drive->ready_to_drive;
  msg.ams_light = input->shutdown->bms_fault;
  msg.imd_light = input->shutdown->imd_fault;
  msg.hv_light = state->precharge->hv_enabled;

  // TODO
  msg.traction_control = false;

  msg.limp_mode = state->drive->limp_mode;

  // TODO
  msg.lv_warning = false;

  msg.active_aero = state->drive->active_aero;
  msg.regen =
      state->drive->ready_to_drive
      && (input->front_can_node->requested_torque < 0);
  msg.shutdown_esd_drain = input->shutdown->buttons_fault;
  msg.shutdown_bms = input->shutdown->bms_fault;
  msg.shutdown_imd = input->shutdown->imd_fault;
  msg.shutdown_bspd = input->shutdown->bpd_fault;
  msg.shutdown_vcu = state->other->vcu_fault;
  msg.shutdown_precharge = !state->precharge->hv_enabled;
  msg.shutdown_master_reset = input->shutdown->master_reset;
  msg.shutdown_driver_reset = input->shutdown->driver_reset;

  msg.heartbeat_front_can_node_dead =
      !Input_device_alive(input, FRONT_CAN_NODE_LIVENESS);
  msg.heartbeat_rear_can_node_dead =
      !Input_device_alive(input, REAR_CAN_NODE_LIVENESS);
  msg.heartbeat_bms_dead = !Input_device_alive(input, BMS_LIVENESS);
  msg.heartbeat_dash_dead = !Input_device_alive(input, DASH_LIVENESS);
  msg.heartbeat_mc_dead = !Input_device_alive(input, MC_LIVENESS);
  msg.heartbeat_current_sensor_dead =
      !Input_device_alive(input, CURRENT_SENSOR_LIVENESS);

  msg.tsms_off = state->other->tsms_fault;
  msg.reset_latch_open = state->other->master_reset_fault;
  msg.precharge_running = state->precharge->precharge_begun;

  msg.master_reset_not_initialized = !state->other->master_reset_initialized;
  msg.driver_reset_not_initialized = !state->other->driver_reset_initialized;
  msg.lv_battery_voltage = input->shutdown->lv_voltage;
  Can_Vcu_DashHeartbeat_Write(&msg);
}

void send_bms_msg(Input_T *input, State_T *state) {
  Can_Vcu_BmsHeartbeat_T msg;
  msg.alwaysTrue = true;
  Can_Vcu_BmsHeartbeat_Write(&msg);
}

void send_torque_cmd_msg(Input_T *input, State_T *state) {
  Can_Vcu_MCTorque_T msg;
  msg.torque_cmd = input->front_can_node->requested_torque;

  Can_Vcu_MCTorque_Write(&msg);
}

void send_mc_request_msg(MC_Request_Type type) {

  Can_Vcu_MCRequest_T msg;
  msg.requestType = Types_MC_Request_to_MC_Reg(type);
  msg.period = 0;

  // Serial.println("Transmit Type: VCU=" + String(type) + ", MC=" + String(msg.requestType));
  Can_Vcu_MCRequest_Write(&msg);
}

void handle_pins(Pin_Output_T *pin) {
  if (pin->fan == Action_ON) {
    digitalWrite(FAN_PIN_OUT, HIGH);
    pin->fan = Action_NONE;
  } else if (pin->fan == Action_OFF) {
    digitalWrite(FAN_PIN_OUT, LOW);
    pin->fan = Action_NONE;
  }

  if (pin->brake_light == Action_ON) {
    digitalWrite(BRAKE_LIGHT_PIN_OUT, HIGH);
    pin->brake_light = Action_NONE;
  } else if (pin->brake_light == Action_OFF) {
    digitalWrite(BRAKE_LIGHT_PIN_OUT, LOW);
    pin->brake_light = Action_NONE;
  }

  if (pin->precharge == Action_ON) {
    digitalWrite(PRECHARGE_PIN_OUT, HIGH);
    pin->precharge = Action_NONE;
  } else if (pin->precharge == Action_OFF) {
    digitalWrite(PRECHARGE_PIN_OUT, LOW);
    pin->precharge = Action_NONE;
  }

  if (pin->mc_enable == Action_ON) {
    digitalWrite(MC_ENABLE_PIN_OUT, HIGH);
    pin->mc_enable = Action_NONE;
  } else if (pin->mc_enable == Action_OFF) {
    digitalWrite(MC_ENABLE_PIN_OUT, LOW);
    pin->mc_enable = Action_NONE;
  }

  if (pin->wing == Action_ON) {
    digitalWrite(WING_PIN_OUT, HIGH);
    pin->wing = Action_NONE;
  } else if (pin->wing == Action_OFF) {
    digitalWrite(WING_PIN_OUT, LOW);
    pin->wing = Action_NONE;
  }

  // NOTE THE SWITCHING OF HIGH AND LOW HERE
  if (pin->vcu_fault == Action_ON) {
    digitalWrite(VCU_FAULT_PIN_OUT, LOW);
    pin->vcu_fault = Action_NONE;
  } else if (pin->vcu_fault == Action_OFF) {
    digitalWrite(VCU_FAULT_PIN_OUT, HIGH);
    pin->vcu_fault = Action_NONE;
  }
}

