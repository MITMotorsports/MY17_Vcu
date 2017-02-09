#include "Vcu.h"

#include "Can_Controller.h"

#include "Dispatch_Controller.h"

void setup() {
  // Dispatcher does all the heavy lifting
  // Dispatcher().begin();
  // TODO restore functionality to dispatcher
  Serial.begin(115200);
  Serial.println("Started VCU");
}

void loop() {
  
}

