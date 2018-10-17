#include <assert.h>
#include "steppers.h"

static char opName[] = "line";

Steppers::Steppers(Fifo* fifo) {
  //assert(fifo != NULL);
  _fifo = fifo;
}

void Steppers::start(long steps, long distances[]){
  _dirMask = 0;

  for(int i=0; i<STEPPER_COUNT; ++i) {
   long dist = distances[i];
   byte dirBit = 0;
   if(dist < 0){
     dist = -dist;
     dirBit = 1;
   }
    _steppers[i].start(dist, steps);
    _dirMask <<= 1;
    _dirMask = _dirMask | dirBit;
  }
}

char* Steppers::name() {
  return opName;
}

bool Steppers::isRunning(){
  bool isRunning = false;
  for(int i=0; i<STEPPER_COUNT; ++i) {
    isRunning |= _steppers[i].isRunning();
  }
  return isRunning;
}

void Steppers::step(){
  byte value = _dirMask;
  for(int i=0; i<STEPPER_COUNT; ++i) {
    byte stepBit = ( _steppers[i].step()) ? 1 : 0;
    value <<=1;
    value = value | stepBit;
  }

  _fifo->add(value);
}

void Steppers::stop(){
  for(int i=0; i<STEPPER_COUNT; ++i) {
    _steppers[i].stop();
  }
  
}

