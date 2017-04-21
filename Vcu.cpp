#include "Vcu.h"

#include <Arduino.h>

#include "Dispatcher.h"

void setup() {
  // Dispatcher does all the heavy lifting
  Dispatch_begin();
}
