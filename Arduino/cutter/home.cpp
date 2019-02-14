#include "home.h"

static char opName[] = "home";


Home::Home(Fifo* fifo, Hardware* hardware) {
  _fifo = fifo;
  _hardware = hardware;
  _stopped = true;
  _backoff = false;
  _backoffCount = BACKOFF_PULSES;
}

char* Home::name(){
  return opName;
}

bool Home::isRunning(){
  return !_stopped;
}

void Home::step(){
  if(!_fifo->isFull()) {
    if(_backoff) {
      _fifo->add(0xFF); // step all axes away from home.
       if(--_backoffCount <= 0) {
        _stopped = true;
        _hardware->enableLimits(true); // re-enable.
      }
    } else {
      if( _hardware->getLimits() == 0x0F) {
        _backoff = true;
        _fifo->clear();
        _hardware->enableLimits(false); // so can move axes away.
      } else {
        _fifo->add(0x0F); // step all axes towards home.
      }
    }
  }
}

void Home::stop(){
  _stopped = true;
  _fifo->clear();
}

void Home::start(){
  _stopped = false;
  _backoff = false;
  _backoffCount = BACKOFF_PULSES;

}

