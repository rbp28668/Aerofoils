#ifndef WIREOFF_HANDLER_H
#define WIREOFF_HANDLER_H

#include "command_handler.h"

class Hardware;

// * X - wire off
struct wireOffT : public commandT{
};




/*
 * CommandHandler to run the WireOff command.
 */
class WireOffHandler : public CommandHandler {
private:
  Hardware* _hardware;

public:
  WireOffHandler(Hardware* hardware);
  virtual size_t getMessageSize() { return sizeof(wireOffT); }
  virtual bool parseInto(char* msg, byte* buffer);
  virtual Operation* process(byte* msg);
  virtual char* getDescription();
};

#endif


