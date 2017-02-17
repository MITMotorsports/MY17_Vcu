#ifndef SEG_DISPLAY_H
#define SEG_DISPLAY_H

#include <Arduino.h>

class Seg_Display {
  public:
    static void displayDigit(uint8_t val);
    static void blank();
    static void begin();
  private:
    static uint8_t numberToPort(uint8_t number);
    static void setRegister(uint8_t number);

};

#endif //SEG_DISPLAY_H
