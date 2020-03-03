#include "command_handler.h"


// Static array of handlers and number of active handlers.
CommandHandler* CommandHandler::_handlers[MAX_HANDLERS];
int CommandHandler::_handlerCount = 0;

void CommandHandler::registerHandler(CommandHandler* handler){
  if(_handlerCount < MAX_HANDLERS) {
    handler->_index = (byte)_handlerCount;
    _handlers[_handlerCount++] = handler;
  }
}

CommandHandler* CommandHandler::lookup(byte idx){
  if(idx <0 || idx >= _handlerCount){
    idx = 0;  // should refer to the abort handler!
  }
  return _handlers[idx];
}

// Look up a handler by command character. May return null.
CommandHandler* CommandHandler::find(char cmd){
  CommandHandler* handler = 0;
  for(int i=0; i<_handlerCount; ++i) {
    if(_handlers[i]->getCommandChar() == cmd){
      handler = _handlers[i];
      break; 
    }
  }
  return handler;  // maybe null
}

CommandHandler::CommandHandler(char cmd) : _command(cmd) { }

byte CommandHandler::hexDig(char ch){
  if(ch >= '0' && ch <= '9'){
    return ch - '0';
  }
  if(ch >= 'A' && ch <= 'F'){
    return ch - 'A' + 10;
  }
  if(ch >= 'a' && ch <= 'f'){
    return ch - 'a' + 10;
  }
  return 0;
}

byte CommandHandler::hexByte(char* str){
  return (hexDig(str[0]) << 4) | hexDig(str[1]);
}

long CommandHandler::hexLong(char* str){
  long acc = 0;
  for(int i=0; i<8; ++i){
    acc <<= 4;
    acc |= hexDig(*str++);
  }
  return acc;
}
