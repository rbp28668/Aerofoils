#include "command_processor.h"
#include "command_handler.h"
#include "command_queue.h"
#include "operation.h"
#include "fifo.h"

#include "line_handler.h"

CommandProcessor::CommandProcessor(CommandQueue* queue, Fifo* fifo, Operation** currentOperation) {
  _commandQueue = queue;
  _fifo = fifo;
  _currentOperation = currentOperation;
}

bool CommandProcessor::process(char* message, int nBytes){

  
  bool ok = false;
  if(nBytes > 0) {


    // Look for any imediate commands
    switch(*message){

    case '?':
      showCommands();
      ok = true;
      break;
      
    case 'P':   // ping, will let host know queue state, otherwise no op.
      ok = true;
      break;

    case 'A':   // stop everything!
      _commandQueue->clear();
      (*_currentOperation)->stop();
      _fifo->clear();
      ok = true;
      break;


    default: 
      if(_commandQueue->isFull()){
        return false; // error to send a command that needs to be queued when queue full.
      } else {
        CommandHandler* handler = CommandHandler::find(*message);
        if(handler == 0) {
          return false;  // no valid handler
        }
        ok = handler->parseInto(message, _commandQueue->addCommand(handler));
      }
      break;
    }
  }
  return ok;
  
}

void CommandProcessor::showCommands(){
  Serial.println("? - show this message");
  Serial.println("P - ping, show queue state");
  Serial.println("A - abort now and clear queues");
  for(int i=0; i<CommandHandler::getCount(); ++i){
    CommandHandler* handler = CommandHandler::lookup(i);
    Serial.print(handler->getCommandChar());
    Serial.print(" - ");
    Serial.println(handler->getDescription());
  }
  Serial.println("--------------");
  Serial.println(sizeof(lineT));
}
