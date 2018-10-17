#include "home_handler.h"
#include "home.h"

static char description[] = "Drives the steppers to their home positions";

HomeHandler::HomeHandler(Home* home) 
: CommandHandler('H') {
  _home = home;
}

bool HomeHandler::parseInto(char* msg, byte* buffer){
  if(*msg++ != getCommandChar()) {
    return false;
  }
  homeT* cmd = (homeT*) buffer;
  cmd->_tag = getIndex();
  
  return true;
}

Operation* HomeHandler::process(byte* msg)  {
   _home->start();
   return _home;
}

char* HomeHandler::getDescription(){
  return description;
}

