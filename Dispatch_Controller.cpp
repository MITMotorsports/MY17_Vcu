#include "Dispatch_Controller.h"

//Magic timing library stuff
#include <SoftTimer.h>
#include <DelayRun.h>
#include <Debouncer.h>
#include <RTClib.h>

#include "Can_Controller.h"
#include "Seg_Display.h"

Dispatch_Controller::Dispatch_Controller()
: begun(false)
{

}

// Handle messages as fast as possible
void dispatchPointer(Task*) {
  Dispatcher().dispatch();
}
Task stepTask(0, dispatchPointer);

void Dispatch_Controller::begin() {
  //Make idempotent
  if(begun) {
    return;
  }
  begun = true;

  Serial.begin(115200);
  delay(50);

  // Initialize controllers
  CAN().begin();
  Seg_Display::begin();

  // Start event loop
  SoftTimer.add(&stepTask);
  Serial.println("Started VCU");
}

// Must define instance prior to use
Dispatch_Controller* Dispatch_Controller::instance = NULL;

Dispatch_Controller& Dispatch_Controller::getInstance() {
  if (!instance) {
    instance = new Dispatch_Controller();
    instance->begin();
  }
  return *instance;
}

Dispatch_Controller& Dispatcher() {
  return Dispatch_Controller::getInstance();
}

uint32_t last_segment = 0;
const uint32_t DELAY = 1000000;

void Dispatch_Controller::dispatch() {
  processCanInputs();
}

void Dispatch_Controller::processCanInputs() {
  if (!CAN().msgAvailable()) {
    return;
  }
  Frame frame = CAN().read();
}
