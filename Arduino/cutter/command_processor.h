#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

class CommandQueue;

/*
 * Class to manage top level parsing of commands.  Either these are executed directly or
 * a command is queued.
 */
class CommandProcessor {
private:
  CommandQueue* _commandQueue;
  void showCommands();
public:
  CommandProcessor(CommandQueue* queue);
  bool process(char* message, int nBytes);
};
#endif


