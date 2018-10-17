#ifndef WIREON_HANDLER_H
#define WIREON_HANDLER_H

#include "command_handler.h"

class Hardware;

// * W - wire on
struct wireOnT : public commandT{
};


/*
 * CommandHandler to run the WireOn command.
 */
class WireOnHandler : public CommandHandler {
private:
  Hardware* _hardware;

public:
  WireOnHandler(Hardware* hardware);
  virtual size_t getMessageSize() { return sizeof(wireOnT); }
  virtual bool parseInto(char* msg, byte* buffer);
  virtual Operation* process(byte* msg);
  virtual char* getDescription();
};

#endif

