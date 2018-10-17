#include "command_queue.h"
#include "command_handler.h"


#define HIGH_WATER (BUFFER_SIZE-64)  // ensure that last command doesn't overflow buffer 

CommandQueue::CommandQueue(){
}

// provides a pointer to write nBytes of command data to.
byte* CommandQueue::addCommand(int nBytes){
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
  return _count > HIGH_WATER;
}

Operation* CommandQueue::processNextCommand() {
  Operation* newOp = 0;
  if( !isEmpty()) {
    byte* head = _buffer + _outPtr;
    CommandHandler* cmd = CommandHandler::lookup(*head);
    newOp = cmd->process(head);
    _count -= cmd->getMessageSize();
    _outPtr += cmd->getMessageSize();
    if(_outPtr > HIGH_WATER){
      _outPtr = 0;
    }
  }
  return newOp;
}

