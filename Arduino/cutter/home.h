#ifndef HOME_H
#define HOME_H
#include "operation.h"
#include "fifo.h"
#include "hardware.h"

#define BACKOFF_PULSES (200*16)
/*
 * Operation that drives the steppers to the home position until
 * all the limit switches are activated.  Relies on Hardware
 * ignoring pulses on axes that are already homed.  Whilst this
 * uses hardware object to see whether the axes are homed it pushes
 * data into the fifo to ensure correct timing of the pulses.  Once
 * all switches are set it changes state to backoff, disables the
 * switch checking on the hardware and drives away from the switches
 * for BACKOFF_PULSES pulses.
 */
class Home : public Operation {
private:
  Fifo* _fifo;
  Hardware* _hardware;
  int _backoffCount;
  bool _stopped;
  bool _backoff;
  
public:
  Home(Fifo* fifo, Hardware* hardware);
  virtual char* name();
  virtual bool isRunning();
  virtual void step();
  virtual void stop();
  void start();
};
#endif
