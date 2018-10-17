#include "wireoff_handler.h"
#include "hardware.h"

static char description[] = "Turn off hot wire";

WireOffHandler::WireOffHandler(Hardware* hardware)
: CommandHandler('X') {
  _hardware = hardware;
}

bool WireOffHandler::parseInto(char* msg, byte* buffer){
  if(*msg++ != getCommandChar()) {
    return false;
  }
  wireOffT* cmd = (wireOffT*) buffer;
  cmd->_tag = getIndex();
  return true;
}

Operation* WireOffHandler::process(byte* msg){
  _hardware->enableHotWire(false);
  return 0;
}

char* WireOffHandler::getDescription(){
  return description;
}

