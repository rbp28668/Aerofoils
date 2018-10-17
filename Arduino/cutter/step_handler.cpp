#include "step_handler.h"
#include "hardware.h"

static char description[] = "Single step - DB where D is 4 bits of direction, P are pulse bits (1 to pulse), both hex digits";
StepHandler::StepHandler(Hardware* hardware) 
: CommandHandler('S') {
  _hardware = hardware;
}

bool StepHandler::parseInto(char* msg, byte* buffer){
  if(*msg++ != getCommandChar()) {
    return false;
  }

  stepT* cmd = (stepT*) buffer;
  cmd->_tag = getIndex();
  cmd->_step = hexByte(msg);
  return true;
}

Operation* StepHandler::process(byte* msg)  {
  stepT* cmd = (stepT*)msg;
  _hardware->step(cmd->_step);
  return 0;
}

char* StepHandler::getDescription(){
  return description;
}

