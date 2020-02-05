#include "command_queue.h"
#include "command_handler.h"


#define HIGH_WATER (BUFFER_SIZE-64)  // ensure that last command doesn't overflow buffer 

CommandQueue::CommandQueue()
 : _inPtr(0)
 , _outPtr(0)
 , _count(0)
{
}

// provides a pointer to write nBytes of command data to.
byte* CommandQueue::addCommand(CommandHandler* cmd){
  int nBytes = cmd->getMessageSize();
  byte* head = _buffer + _inPtr;
  _count += nBytes;
  _inPtr += nBytes;
  if(_inPtr > HIGH_WATER) {
    _inPtr = 0;
  }

  return head;
}

bool CommandQueue::isEmpty() {
  return _count == 0;
}

bool CommandQueue::isFull() {
  return _count >= HIGH_WATER;
}

void CommandQueue::clear() {
  _inPtr = _outPtr = _count = 0;
}

Operation* CommandQueue::processNextCommand() {
  Operation* newOp = 0;
  if( !isEmpty()) {
    byte* head = _buffer + _outPtr;
    CommandHandler* cmd = CommandHandler::lookup(*head);
    int nBytes = cmd->getMessageSize();
    
    _count -= nBytes;
    _outPtr += nBytes;
    if(_outPtr > HIGH_WATER){
      _outPtr = 0;
    }
     newOp = cmd->process(head);
  }
  return newOp;
}
