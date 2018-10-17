#include "home.h"

static char opName[] = "home";


Home::Home(Fifo* fifo, Hardware* hardware) {
  _fifo = fifo;
  _hardware = hardware;
  _stopped = true;
}

char* Home::name(){
  return opName;
}

bool Home::isRunning(){
  return !_stopped && _hardware->getLimits() != 0x0F;
}

void Home::step(){
  if(!_fifo->isFull()) {
    _fifo->add(0x0F); // step all axes towards home.
  }
}

void Home::stop(){
  _stopped = true;
  _fifo->clear();
}

void Home::start(){
  _stopped = false;
}

