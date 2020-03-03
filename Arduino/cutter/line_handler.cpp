#include "line_handler.h"
#include "steppers.h"

static char description[] = "Line in 4 dimensions SSSSSSSSAAAAAAAABBBBBBBBCCCCCCCCDDDDDDDD where S gives total number of steps and A-D are axes. All in hex, 2s complement";

struct DataT {
  long _steps;
  long _axes[4];
};

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

  // Copy of data to avoid any alignment issues. lineT is packed(1).
  DataT data;
  data._steps = hexLong(msg);
  msg += 2 * sizeof(long); // 2 chars per byte
  for(int i=0; i<4; ++i) {
    data._axes[i] = hexLong(msg);
    msg += 2 * sizeof(long); 
  }
  memcpy( &(cmd->_steps), &data, sizeof(data));
  return true;
}

Operation* LineHandler::process(byte* msg)  {
  lineT* cmd = (lineT*)msg;
  DataT data;
  memcpy(&data, &(cmd->_steps), sizeof(data));
  _steppers->start(data._steps, data._axes);
  return _steppers;
}

char* LineHandler::getDescription(){
  return description;
}
