#ifndef DISABLE_HANDLER_H
#define DISABLE_HANDLER_H

#include "command_handler.h"

class Hardware;

// * D - disable motors
struct disableMotorsT : public commandT{
};


/*
 * CommandHandler to run the DisableMotors command.
 */
class DisableHandler : public CommandHandler {
private:
  Hardware* _hardware;

public:
  DisableHandler(Hardware* hardware);
  virtual size_t getMessageSize() { return sizeof(disableMotorsT); }
  virtual bool parseInto(char* msg, byte* buffer);
  virtual Operation* process(byte* msg);
  virtual char* getDescription();

};

#endif

