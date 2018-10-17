#include "fifo.h"

Fifo::Fifo(){
  _count = 0;
  _inPtr = 0;
  _outPtr = 0;
  analogWriteResolution(12);
}

void Fifo::add(byte val){
  noInterrupts();
  if(_count < FIFO_SIZE) {
    ++_count;
    _buffer[_inPtr] = val;
    _inPtr = (_inPtr + 1) % FIFO_SIZE;
  }
  interrupts();
  analogWrite(A14, _count);
}

// Call from ISR only
byte Fifo::get(){
  byte val = 0;
  if(_count > 0) {
    --_count;
    val = _buffer[_outPtr];
    _outPtr = (_outPtr + 1) % FIFO_SIZE;
  }
  analogWrite(A14, _count);
  return val;
}


bool Fifo::isEmpty(){
  return _count == 0;
}

bool Fifo::isFull(){
  return _count == FIFO_SIZE;
}

bool Fifo::isHalfFull(){
  return _count >= FIFO_SIZE/2;
}

int Fifo::used(){
  return _count;
}

void Fifo::clear() {
  noInterrupts();
  _count = 0;
  _inPtr = 0;
  _outPtr = 0;
  interrupts();
}


