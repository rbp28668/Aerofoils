#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "WProgram.h"

// 20k buffer. Note that 40k is too much and we run out of stack space.
#define BUFFER_SIZE 20000

class CommandHandler;
class Operation;

/*
 * Provides a FIFO buffer for commands.  Allows the host to keep a queue of
 * commands to be executed so that cutting is seamless.
 */
class CommandQueue {

private:
  byte _buffer[BUFFER_SIZE];
  long _inPtr; // offset into buffer for writes
  long _outPtr; // offset into buffer for reads
  long _count; // number of bytes used.
  
public:
  CommandQueue();
  byte* addCommand(CommandHandler* cmd);
  bool isEmpty();
  bool isFull();
  void clear();
  Operation* processNextCommand();

};


#endif
