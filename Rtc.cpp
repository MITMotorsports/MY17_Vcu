#include "Rtc.h"

void Rtc::begin()
{
  Wire.begin();
}

void Rtc::setTime(Time& time) {
  // sets time and date data to DS3231
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(time.second)); // set seconds
  Wire.write(decToBcd(time.minute)); // set minutes
  Wire.write(decToBcd(time.hour)); // set hours
  Wire.write(decToBcd(time.dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(time.dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(time.month)); // set month
  Wire.write(decToBcd(time.year)); // set year (0 to 99)
  Wire.endTransmission();
}

void Rtc::setTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint8_t year) {
  // sets time and date data to DS3231
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

Time Rtc::getTime() {
  Time result;
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(RTC_I2C_ADDRESS, (uint8_t)7);
  // request seven uint8_ts of data from DS3231 starting from register 00h
  result.second = bcdToDec(Wire.read() & 0x7f);
  result.minute = bcdToDec(Wire.read());
  result.hour = bcdToDec(Wire.read() & 0x3f);
  result.dayOfWeek = bcdToDec(Wire.read());
  result.dayOfMonth = bcdToDec(Wire.read());
  result.month = bcdToDec(Wire.read());
  result.year = bcdToDec(Wire.read());
  return result;
}

void Rtc::getTime(uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *dayOfWeek, uint8_t *dayOfMonth, uint8_t *month, uint8_t *year) {
  Wire.beginTransmission(RTC_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(RTC_I2C_ADDRESS, (uint8_t)7);
  // request seven uint8_ts of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

uint8_t Rtc::decToBcd(uint8_t val)
{
  return( (val/10*16) + (val%10) );
}

uint8_t Rtc::bcdToDec(uint8_t val)
{
  return( (val/16*10) + (val%16) );
}
