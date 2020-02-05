#include "abort_handler.h"
#include "hardware.h"
#include "fifo.h"
#include "operation.h"
#include "command_queue.h"

static char description[] = "Abort current operation and clear queues";

AbortHandler::AbortHandler(CommandQueue* commands, Fifo* fifo, Hardware* hardware, Operation** currentOperation)
: CommandHandler('Z') {
  _commands = commands;
  _fifo = fifo;
  _hardware = hardware;
  _currentOperation = currentOperation;
}

bool AbortHandler::parseInto(char* msg, byte* buffer){
  if(*msg++ != getCommandChar()) {
    return false;
  }
  abortT* cmd = (abortT*) buffer;
  cmd->_tag = getIndex();
  return true;
}

Operation* AbortHandler::process(byte* msg){
  _hardware->setEnabled(false);
  _fifo->clear();
  (*_currentOperation)->stop();
  _commands->clear();
  return 0;
}

char* AbortHandler::getDescription(){
  return description;
}
