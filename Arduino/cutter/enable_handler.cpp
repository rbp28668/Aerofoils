#include "enable_handler.h"
#include "hardware.h"

static char description[] = "Enable motors";

EnableHandler::EnableHandler(Hardware* hardware)
: CommandHandler('E') {
  _hardware = hardware;
}

bool EnableHandler::parseInto(char* msg, byte* buffer){
  if(*msg++ != getCommandChar()) {
    return false;
  }
  enableMotorsT* cmd = (enableMotorsT*) buffer;
  cmd->_tag = getIndex();
  return true;
}

Operation* EnableHandler::process(byte* msg){
  _hardware->setEnabled(true);
  return 0;
}

char* EnableHandler::getDescription(){
  return description;
}

