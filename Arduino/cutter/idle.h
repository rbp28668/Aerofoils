#ifndef IDLE_H
#define IDLE_H
#include "operation.h"
class Idle : public Operation {
public:

  virtual char* name();
  virtual bool isRunning();
  virtual void step();
  virtual void stop();
};

#endif
