#ifndef DISPATCH_CONTROLLER_H
#define DISPATCH_CONTROLLER_H

#include <stdbool.h>

#include <SoftTimer.h>
#include <DelayRun.h>
#include <Debouncer.h>

void Dispatch_begin();
void Dispatch_run(Task*);

#endif // DISPATCH_CONTROLLER_H
