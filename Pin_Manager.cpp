#include "Pin_Manager.h"

#include "Pins.h"

void Pin_Manager::setup_pins() {
  // Do NOT setup MCP_CS_PIN - CAN library does it
  setup_input_pins();
  setup_output_pins();
}

void Pin_Manager::setup_output_pins() {
  pinMode(VCU_FAULT_PIN_OUT, OUTPUT);
  pinMode(PRECHARGE_PIN_OUT, OUTPUT);
  pinMode(FAN_PIN_OUT, OUTPUT);
  pinMode(WING_PIN_OUT, OUTPUT);
  pinMode(BRAKE_LIGHT_PIN_OUT, OUTPUT);
  pinMode(MC_ENABLE_PIN_OUT, OUTPUT);
}

void Pin_Manager::setup_input_pins() {
  pinMode(BPD_FAULT_PIN_IN, INPUT);
  pinMode(IMD_FAULT_PIN_IN, INPUT);
  pinMode(BMS_FAULT_PIN_IN, INPUT);
  pinMode(ESD_DRAIN_FAULT_PIN_IN, INPUT);
  pinMode(DRIVER_RESET_FAULT_PIN_IN, INPUT);
  pinMode(MASTER_RESET_FAULT_PIN_IN, INPUT);
  pinMode(LOW_SIDE_MEASURE_PIN_IN, INPUT);
  pinMode(LV_MEASURE_PIN_IN, INPUT);
}
