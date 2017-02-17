#include "Seg_Display.h"
#include "Pins.h"
#include "assert.h"

void Seg_Display::begin() {
  blank();
}

void Seg_Display::displayDigit(uint8_t val) {
  assert(val >= 0);
  assert(val <= 9);
  setRegister(val);
  // Serial.println(val);
}

// Setting all registers to 1 makes a blank
void Seg_Display::blank() {
  setRegister(10);
  // Serial.println("Blank");
}

uint8_t Seg_Display::numberToPort(uint8_t input) {
  assert(input >= 0x00);
  assert(input <= 0x0F);
  uint8_t result = 0x00;
  if ((input & 0x01) == 0x01) {
    // Serial.println("Disp A");
    result |= 1 << DISP_A;
  }
  if ((input & 0x02) == 0x02) {
    // Serial.println("Disp B");
    result |= 1 << DISP_B;
  }
  if ((input & 0x04) == 0x04) {
    // Serial.println("Disp C");
    result |= 1 << DISP_C;
  }
  if ((input & 0x08) == 0x08) {
    // Serial.println("Disp D");
    result |= 1 << DISP_D;
  }
  // Serial.println(result);
  return result;
}

void Seg_Display::setRegister(uint8_t input) {
  uint8_t or_value = numberToPort(input);
  uint8_t and_value = ~(numberToPort(15));
  uint8_t result = ((SEG_DISPLAY_REG & and_value) | or_value);
  SEG_DISPLAY_REG = result;
}
