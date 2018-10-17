#ifndef LINE_HANDLER_H
#define LINE_HANDLER_H

#include "command_handler.h"

class Steppers;


// Exernalised state
struct lineT : public commandT{
  long _steps;
  long _axes[4];
};

// Lssssaaaabbbbccccdddd Line, a,b,c,d over s steps in hex
class LineHandler : public CommandHandler {
private:
  Steppers* _steppers;
public:
  LineHandler(Steppers* _steppers);
  virtual size_t getMessageSize() { return sizeof(lineT); }
  virtual bool parseInto(char* msg, byte* buffer);
  virtual Operation* process(byte* msg);
  virtual char* getDescription();
};

#endif

