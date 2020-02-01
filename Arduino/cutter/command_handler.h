#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "Arduino.h"

class Operation;

#define MAX_HANDLERS 16

// This is the base class for binary representations of a command
struct commandT{
  char _tag;
};

/*
 * CommandHandlers provide the means of parsing input for each command and
 * (where appropriate) placing a binary representation of the command into
 * the buffer provided by the command queue.  It also keeps a static table
 * of commands a. for lookup of the command by letter code, but also b. for
 * fast lookup by index so the command can be executed. Hence, whilst the
 * CommandHandlers do all the work, the representation of each command can
 * be kept lightweight (flyweight pattern?) which should enable the 
 * command queue to maximise the number of stored commands.
 */
class CommandHandler {
private:
  static CommandHandler* _handlers[MAX_HANDLERS];
  static int _handlerCount;
  
  byte _index;  // used to map binary representation to handler.
  char _command;
  byte hexDig(char ch);

protected:
  byte hexByte(char* str);
  long hexLong(char* str);
  byte getIndex() { return _index;}
  
public:

  CommandHandler(char cmd);
  char getCommandChar() { return _command;}
  virtual size_t getMessageSize() = 0;
  virtual bool parseInto(char* msg, byte* buffer) = 0;
  virtual Operation* process(byte* msg) = 0;
  virtual char* getDescription() = 0;
  static void registerHandler(CommandHandler* handler);
  static CommandHandler* lookup(byte idx);
  static CommandHandler* find(char cmd);
  static int getCount() {return _handlerCount;}
  
};

#endif
