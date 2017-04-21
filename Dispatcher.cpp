#include "Dispatcher.h"

//Magic timing library stuff
#include <MY17_Can_Library.h>

#include "Input.h"
#include "Pins.h"
#include "Pin_Manager.h"
#include "Seg_Display.h"

uint32_t last_segment = 0;
const uint32_t DELAY = 1000000;

/******* BEGIN forward declarations ***********/

void initialize_structs();

void process_inputs(Input_T *input);

void process_can_outputs();

void can_test_print(const char * print);

Task Task_Dispatch_run(0, Dispatch_run);

/******** END forward declarations ***********/

/*********** BEGIN state structs *************/

static Input_T input;

static Can_Node_Input_T can_node_input;
static Dash_Input_T dash_input;
static Bms_Input_T bms_input;
static Mc_Input_T mc_input;
static Current_Sensor_Input_T current_sensor_input;
static Shutdown_Input_T shutdown_input;

/************* END state structs *************/

/********** BEGIN public method definitions *********/

void Dispatch_begin() {
  // Initialize Serial
  Serial.begin(115200);
  delay(50);

  // Initialize all pins besides CAN
  Pin_Manager::setup_pins();

  // Initialize CAN
  Can_Init(500000);

  // Initialize controllers
  Seg_Display::begin();

  // Initialize state variables
  initialize_structs();

  // Start event loop
  Serial.println("Started VCU");
  SoftTimer.add(&Task_Dispatch_run);
}

void Dispatch_run(Task*) {
  process_inputs(&input);
}

void process_inputs(Input_T *input) {
  input->msTicks = millis();
  Input_process_can(input);
  Input_process_pins(input);
}

void can_test_print(const char * text) {
  Serial.print(text);
}

void initialize_structs() {
  input.can_node = &can_node_input;
  input.dash = &dash_input;
  input.bms = &bms_input;
  input.mc = &mc_input;
  input.current_sensor = &current_sensor_input;
  input.shutdown = &shutdown_input;
  Input_initialize(&input);
}

/********** END method definitions ***********/
