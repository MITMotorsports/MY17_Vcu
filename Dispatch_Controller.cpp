#include "Dispatch_Controller.h"

//Magic timing library stuff
#include <MY17_Can_Library.h>
#include <MY17_Can_Library_Test.h>

#include "Seg_Display.h"
#include "Pin_Manager.h"
#include "Pins.h"

uint32_t last_segment = 0;
const uint32_t DELAY = 1000000;

/******* BEGIN forward declarations ***********/
void process_can_inputs();
void process_pin_inputs();

void process_can_outputs();

void can_test_print(const char * print);

Task Task_Dispatch_run(0, Dispatch_run);
/******** END forward declarations ***********/

void Dispatch_begin() {
  Pin_Manager::setup_pins();
  Serial.begin(115200);
  delay(50);

  Can_Init(500000);
  Can_All_Tests(can_test_print);

  // Initialize controllers
  Seg_Display::begin();

  // Start event loop
  SoftTimer.add(&Task_Dispatch_run);
  Serial.println("Started VCU");
}

void Dispatch_run(Task*) {
  process_can_inputs();
}

void process_can_inputs() {
  Can_MsgID_T msgID = Can_MsgType();
  if (msgID == Can_No_Msg) {
    return;
  }
  if (msgID == Can_FrontCanNode_DriverOutput_Msg) {
      Can_FrontCanNode_DriverOutput_T result;
      Can_FrontCanNode_DriverOutput_Read(&result);
  }
  else if (msgID == Can_Unknown_Msg) {
    Frame frame;
    Can_RawRead(&frame);
    if (frame.id == 0x69) {
      uint8_t first = frame.data[0];
      if (first == 1) {
        digitalWrite(MC_ENABLE_PIN_OUT, HIGH);
      } else if (first == 0) {
        digitalWrite(MC_ENABLE_PIN_OUT, LOW);
      }
    }
  }
}

void can_test_print(const char * input) {
  Serial.print(input);
}

