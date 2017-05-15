#include "Output.h"

#include <Arduino.h>

#include "Input.h"
#include "Pins.h"

void handle_can(Input_T *input, State_T *state, Can_Output_T *can);
void handle_pins(Pin_Output_T *output);
void handle_onboard(Input_T *input, State_T *state, Onboard_Output_T *onboard);

void send_dash_msg(Input_T *input, State_T *state);
void send_bms_msg(Input_T *input, State_T *state);
void send_torque_cmd_msg(Input_T *input, State_T *state);

void print_data(String prefix, int32_t data, String unit, uint32_t msTicks);

void Output_initialize(Output_T *output) {
  output->can->send_dash_msg = false;
  output->can->send_bms_msg = false;
  output->can->send_mc_single_request_msg = false;
  output->can->send_mc_permanent_request_msg = false;
  output->can->send_torque_cmd = false;

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

  // TODO
  output->xbee->temp = false;
}

void Output_empty_output(Input_T *input, State_T *state, Output_T *output) {
  handle_can(input, state, output->can);
  handle_pins(output->pin);
  handle_onboard(input, state, output->onboard);
}

void handle_onboard(Input_T *input, State_T *state, Onboard_Output_T *onboard) {
  Current_Sensor_Input_T *sensor = input->current_sensor;

  if (onboard->write_current_log) {
    onboard->write_current_log = false;
    print_data("current", sensor->current_mA, "mA", sensor->last_current_ms);
  }
  if (onboard->write_voltage_log) {
    onboard->write_voltage_log = false;
    print_data("voltage", sensor->voltage_mV, "mV", sensor->last_voltage_ms);
  }
  if (onboard->write_power_log) {
    onboard->write_power_log = false;
    print_data("power", sensor->power_W, "W", sensor->last_power_ms);
  }
  if (onboard->write_energy_log) {
    onboard->write_energy_log = false;
    print_data("energy", sensor->energy_Wh, "Wh", sensor->last_energy_ms);
  }

  if (onboard->write_front_can_log) {
    Front_Can_Node_Input_T *front_can = input->front_can_node;
    const uint32_t last_updated = front_can->last_updated;

    onboard->write_front_can_log = false;
    print_data("torque", front_can->requested_torque, "int16_t", last_updated);
    print_data("brake", front_can->brake_pressure, "uint8_t", last_updated);
  }
}

void print_data(String prefix, int32_t data, String unit, uint32_t msTicks) {
  String line;
  line.concat(prefix);
  line.concat(", ");
  line.concat(data);
  line.concat(", ");
  line.concat(unit);
  line.concat(", ");
  line.concat(msTicks);
  Serial3.println(line);
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

  // TODO
  msg.lv_battery_voltage = 138;

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

  // HACK Motor is mounted backwards lolz
  msg.torque_cmd = -1 * msg.torque_cmd;

  Can_Vcu_MCTorque_Write(&msg);
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

