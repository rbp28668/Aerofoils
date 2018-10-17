#ifndef FIFO_H
#define FIFO_H


#include "WProgram.h"

#define FIFO_SIZE 2048

/*
 * Fifo class to provide some buffering for the raw stepper data.  This allows the timer
 * interrupt service to pull a byte (4 bits of direction, 4 bits of step/not step) of data
 * off and send it to the hardware on each tick.   Meanwhile whatever operation is running can 
 * push data into the fifo to try to keep it topped up.
 */
class Fifo {
  
private:
  byte _buffer[FIFO_SIZE];
  volatile int _inPtr;
  volatile int _outPtr;
  volatile int _count;
public:
  Fifo();
  void add(byte val);
  byte get();
  bool isEmpty();
  bool isFull();
  bool isHalfFull();
  void clear();
  int used();
};
#endif
