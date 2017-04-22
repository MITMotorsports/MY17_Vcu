#include "Dispatcher.h"

//Magic timing library stuff
#include <MY17_Can_Library.h>

#include "Input.h"
#include "State.h"
#include "Output.h"

#include "Pins.h"
#include "Pin_Manager.h"
#include "Seg_Display.h"
#include "State.h"

uint32_t last_segment = 0;
const uint32_t DELAY = 1000000;

/******* BEGIN forward declarations ***********/

void initialize_structs();

void fill_input(Input_T *input);
void update_state(Input_T *input, State_T *state, Output_T *output);
void empty_output(Input_T *input, State_T *state, Output_T *output);

Task Task_Dispatch_run(0, Dispatch_run);

/******** END forward declarations ***********/

/*********** BEGIN state structs *************/

static Input_T input;
static State_T state;
static Output_T output;

static Can_Node_Input_T can_node_input;
static Dash_Input_T dash_input;
static Bms_Input_T bms_input;
static Mc_Input_T mc_input;
static Current_Sensor_Input_T current_sensor_input;
static Shutdown_Input_T shutdown_input;

static Precharge_State_T precharge_state;
static Drive_State_T drive_state;
static Message_State_T message_state;
static Other_State_T other_state;

static Can_Output_T can_output;
static Pin_Output_T pin_output;
static Onboard_Output_T onboard_output;
static Xbee_Output_T xbee_output;
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
  fill_input(&input);
  update_state(&input, &state, &output);
  empty_output(&input, &state, &output);
}

void fill_input(Input_T *input) {
  input->msTicks = millis();
  Input_update_can(input);
  Input_update_pins(input);
}

void update_state(Input_T *input, State_T *state, Output_T *output) {
  State_update_state(input, state, output);
}

void empty_output(Input_T *input, State_T *state, Output_T *output) {
  Output_empty_output(input, state, output);
}

void initialize_structs() {
  input.can_node = &can_node_input;
  input.dash = &dash_input;
  input.bms = &bms_input;
  input.mc = &mc_input;
  input.current_sensor = &current_sensor_input;
  input.shutdown = &shutdown_input;
  Input_initialize(&input);

  state.precharge = &precharge_state;
  state.drive = &drive_state;
  state.message = &message_state;
  state.other = &other_state;
  State_initialize(&state);

  output.can = &can_output;
  output.pin = &pin_output;
  output.onboard = &onboard_output;
  output.xbee = &xbee_output;
  Output_initialize(&output);
}

/********** END method definitions ***********/
