#ifndef DISPATCH_CONTROLLER_H
#define DISPATCH_CONTROLLER_H

#include <Arduino.h>
#include <RTClib.h>

class Dispatch_Controller {
  public:
    static Dispatch_Controller& getInstance();
    void begin();
    void dispatch();
  private:
    Dispatch_Controller();
    static Dispatch_Controller *instance;
    bool begun;
    void processCan();
    void processSeg();
    String toBitstring(uint8_t reg);
    RTC_DS1307 rtc;
};

Dispatch_Controller& Dispatcher();

#endif // DISPATCH_CONTROLLER_H
