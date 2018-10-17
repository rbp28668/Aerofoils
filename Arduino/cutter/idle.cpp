#include "idle.h"

static char opName[] = "idle";


char* Idle::name() {
  return opName;
}

bool Idle::isRunning(){
  return false;
}

void Idle::step(){
  // NOP
}


void Idle::stop() {
  // NOP
}

