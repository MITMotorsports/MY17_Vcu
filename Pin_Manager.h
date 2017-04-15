#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H

class Pin_Manager {
  public:
    static void setup_pins();
  private:
    static void setup_input_pins();
    static void setup_output_pins();
};

#endif // _PIN_MANAGER_H
