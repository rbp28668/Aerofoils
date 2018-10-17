#include "wireon_handler.h"
#include "hardware.h"

static char description[] = "Turn on hot wire";

WireOnHandler::WireOnHandler(Hardware* hardware)
: CommandHandler('W') {
  _hardware = hardware;
}

bool WireOnHandler::parseInto(char* msg, byte* buffer){
  if(*msg++ != getCommandChar()) {
    return false;
  }
  wireOnT* cmd = (wireOnT*) buffer;
  cmd->_tag = getIndex();
  return true;
}

Operation* WireOnHandler::process(byte* msg){
  _hardware->enableHotWire(true);
  return 0;
}

char* WireOnHandler::getDescription(){
  return description;
}

