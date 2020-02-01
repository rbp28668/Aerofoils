#ifndef HARDWARE_H
#define HARDWARE_H

#include "Arduino.h"

class Hardware {

private:
  bool _enabled;
  bool _limitsEnabled;
public:
  Hardware();
  void step(byte value);
  void setEnabled(bool enabled);
  bool isEnabled();
  byte getLimits();
  void enableHotWire(bool enabled);
  void enableStatusLed(bool enabled);
  void enableLimits(bool enabled);
};
#endif
