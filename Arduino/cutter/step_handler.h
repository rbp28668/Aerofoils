#ifndef STEP_HANDLER_H
#define STEP_HANDLER_H

#include "command_handler.h"

class Hardware;


// Exernalised state
struct stepT : public commandT{
  byte _step;
};

// Shh  single step, 2 hex digits for dirn & step
class StepHandler : public CommandHandler {
private:
  Hardware* _hardware;
public:
  StepHandler(Hardware* _hardware);
  virtual size_t getMessageSize() { return sizeof(stepT); }
  virtual bool parseInto(char* msg, byte* buffer);
  virtual Operation*  process(byte* msg);
  virtual char* getDescription();
};

#endif

