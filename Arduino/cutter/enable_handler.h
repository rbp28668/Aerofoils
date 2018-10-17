#ifndef ENABLE_HANDLER_H
#define ENABLE_HANDLER_H

#include "command_handler.h"

class Hardware;

// * E - enable motors
struct enableMotorsT : public commandT{
};


/*
 * CommandHandler to run the EnableMotors command.
 */
class EnableHandler : public CommandHandler {
private:
  Hardware* _hardware;

public:
  EnableHandler(Hardware* hardware);
  virtual size_t getMessageSize() { return sizeof(enableMotorsT); }
  virtual bool parseInto(char* msg, byte* buffer);
  virtual Operation* process(byte* msg);
  virtual char* getDescription();
};

#endif

