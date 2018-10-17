#include "disable_handler.h"
#include "hardware.h"

static char description[] = "Disable motors";

DisableHandler::DisableHandler(Hardware* hardware)
: CommandHandler('D') {
  _hardware = hardware;
}

bool DisableHandler::parseInto(char* msg, byte* buffer){
  if(*msg++ != getCommandChar()) {
    return false;
  }
  disableMotorsT* cmd = (disableMotorsT*) buffer;
  cmd->_tag = getIndex();
  return true;
}

Operation* DisableHandler::process(byte* msg){
  _hardware->setEnabled(false);
  return 0;
}

char* DisableHandler::getDescription(){
  return description;
}

