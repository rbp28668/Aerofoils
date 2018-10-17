#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H

#include "WProgram.h"

#define BUFFER_SIZE 40000

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
  byte* addCommand(int nBytes);
  bool isEmpty();
  bool isFull();
  Operation* processNextCommand();

};


#endif

