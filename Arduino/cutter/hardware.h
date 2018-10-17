#ifndef HARDWARE_H
#define HARDWARE_H

#include "WProgram.h"

class Hardware {

private:
  bool _enabled;
public:
  Hardware();
  void step(byte value);
  void setEnabled(bool enabled);
  bool isEnabled();
  byte getLimits();
  void enableHotWire(bool enabled);
  void enableStatusLed(bool enabled);
};
#endif


