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

const uint16_t ID_1 = 1;
const uint16_t ID_2 = 2;
const uint16_t STOP_ID = 3;

bool done = false;
bool printed = false;
uint32_t count_1 = 0;
uint32_t count_2 = 0;
uint32_t count_total = 0;

uint32_t elapsed_time = 0;
uint32_t read_time = 0;

// Handle messages as fast as possible
void dispatchPointer(Task*) {
  if (!done) {
    Dispatcher().dispatch();
  } else if (!printed) {
    printed = true;
    Serial.print("msg_1: ");
    Serial.print(count_1);
    Serial.print(", msg_2: ");
    Serial.print(count_2);
    Serial.print(", elapsed: ");
    Serial.print(elapsed_time);
    Serial.print(", read: ");
    Serial.println(read_time);
  }
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

  rtc.begin();
  Seg_Display::begin();

  // TODO set this when wanting to initialize
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Start event loop
  SoftTimer.add(&stepTask);
  Serial.println("Started VCU");

  Frame start = {.id=4, .body={10}, .len=1};
  CAN().write(start);
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
  count_total++;
  const uint32_t start_time_elapsed = micros();

  if (CAN().msgAvailable()) {
    processCan();
  }

  if (start_time_elapsed - last_segment > DELAY) {
    last_segment = start_time_elapsed;
    processSeg();
  }

  uint32_t end_time_elapsed = micros();
  if (end_time_elapsed > start_time_elapsed) {
    elapsed_time += end_time_elapsed - start_time_elapsed;
  }
}

void Dispatch_Controller::processCan() {
  uint32_t start_time_read = micros();
  Frame frame = CAN().read();
  switch(frame.id) {
    case ID_1:
      count_1++;
      frame.id += 100;
      CAN().write(frame);
      break;
    case ID_2:
      count_2++;
      frame.id += 100;
      CAN().write(frame);
      break;
    case STOP_ID:
      frame.id += 100;
      CAN().write(frame);
      done = true;
      break;
    default:
      Serial.println("Unexpected message");
      done = true;
      break;
  }
  uint32_t end_time_read = micros();
  if (end_time_read > start_time_read) {
    read_time += end_time_read - start_time_read;
  }
}

void Dispatch_Controller::processSeg() {
  const uint32_t curr_time = micros();
  const uint8_t second = (curr_time / DELAY) % 10;
  Seg_Display::displayDigit(second);
  Serial.print("Display ");
  Serial.print(second);
  Serial.print(" at ");
  Serial.print(curr_time);
  Serial.print(" with reg ");
  Serial.print(SEG_DISPLAY_REG);
  Serial.print(", bitstring ");
  Serial.println(toBitstring(SEG_DISPLAY_REG));
  // execute function
}

String Dispatch_Controller::toBitstring(uint8_t reg) {
  String result = "";
  for (int i = 7; i >= 0; i--) {
    if ((reg >> i) & 0x01) {
      result += "1";
    } else {
      result += "0";
    }
  }
  return result;
}
