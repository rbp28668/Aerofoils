#include "line_handler.h"
#include "steppers.h"

static char description[] = "Line in 4 dimensions SSSSSSSSAAAAAAAABBBBBBBBCCCCCCCCDDDDDDDD where S gives total number of steps and A-D are axes. All in hex, 2s complement";

LineHandler::LineHandler(Steppers* steppers) 
: CommandHandler('L') {
  _steppers = steppers;
}

bool LineHandler::parseInto(char* msg, byte* buffer){
  if(*msg++ != getCommandChar()) {
    return false;
  }

  lineT* cmd = (lineT*) buffer;
  cmd->_tag = getIndex();
  cmd->_steps = hexLong(msg);
  msg += 2 * sizeof(long); // 2 chars per byte
  for(int i=0; i<4; ++i) {
    cmd->_axes[i] = hexLong(msg);
    msg += 2 * sizeof(long); 
  }
  return true;
}

Operation* LineHandler::process(byte* msg)  {
  lineT* cmd = (lineT*)msg;
  _steppers->start(cmd->_steps, cmd->_axes);
  return _steppers;
}

char* LineHandler::getDescription(){
  return description;
}

