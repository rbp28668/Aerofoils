#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

class CommandQueue;
class Fifo;
class Operation;

/*
 * Class to manage top level parsing of commands.  Either these are executed directly or
 * a command is queued.
 */
class CommandProcessor {
private:
  CommandQueue* _commandQueue;
  Fifo* _fifo; 
  Operation** _currentOperation;
  
  void showCommands();
public:
  CommandProcessor(CommandQueue* queue,  Fifo* fifo, Operation** currentOperation);
  bool process(char* message, int nBytes);
};
#endif
