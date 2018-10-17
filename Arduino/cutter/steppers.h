#ifndef STEPPERS_H
#define STEPPERS_H
#include "WProgram.h"
#include "operation.h"
#include "fifo.h"
#include "stepper.h"

#define STEPPER_COUNT 4

class Steppers : public Operation {

private:
  Fifo* _fifo;
  byte _dirMask;
  Stepper _steppers[STEPPER_COUNT];

public:
  Steppers(Fifo* fifo);
  void start(long steps, long distances[]);
  virtual char* name();
  virtual bool isRunning();
  virtual void step();
  virtual void stop();
};

#endif
