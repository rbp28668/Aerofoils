#include <assert.h>
#include "stepper.h"


Stepper::Stepper() {
  stop();
}

void Stepper::start(long dist, long steps){
  //assert(dist >= 0);
  //assert(steps >= 0);
  //assert(dist <= steps);

  _dist = dist;
  _steps = steps;
  _count = steps;
  _err = 2*dist - steps;
}

bool Stepper::isRunning(){
  return _count > 0;
}

bool Stepper::step(){
  bool doStep = false;
  --_count;
  if(_err > 0) {
    doStep = true;
    _err = _err - 2 * _steps;
  }
  _err = _err + 2 * _dist;

  return doStep;
}

void Stepper::stop(){
  _dist = 0;
  _steps = 0;
  _count = 0;
  _err = 0;
} 


