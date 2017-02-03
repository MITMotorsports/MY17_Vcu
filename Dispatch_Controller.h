#ifndef DISPATCH_CONTROLLER_H
#define DISPATCH_CONTROLLER_H

class Dispatch_Controller {
  public:
    static Dispatch_Controller& getInstance();
    void begin();
    void dispatch();
  private:
    Dispatch_Controller();
    static Dispatch_Controller *instance;
    bool begun;
};

Dispatch_Controller& Dispatcher();

#endif // DISPATCH_CONTROLLER_H
