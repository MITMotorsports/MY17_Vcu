#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include <Wire.h>

const uint8_t RTC_I2C_ADDRESS = 0x68;

struct Time {
  uint8_t second : 6;
  uint8_t minute : 6;
  uint8_t hour : 4;
  uint8_t dayOfWeek : 3;
  uint8_t dayOfMonth : 5;
  uint8_t month : 4;
  uint8_t year : 7;
};

class Rtc {
  public:
    static void begin();

    // TODO pick one of these two implementations to use

    static void setTime(Time &time);
    static Time getTime();

    static void setTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint8_t year);
    static void getTime(uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *dayOfWeek, uint8_t *dayOfMonth, uint8_t *month, uint8_t *year);
    static uint32_t toUtc(Time& time);

  private:
    static uint8_t decToBcd(uint8_t val);
    static uint8_t bcdToDec(uint8_t val);
};

#endif //RTC_H
