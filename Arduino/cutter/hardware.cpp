#include "hardware.h"


// Pin numbers
// Leave 0 & 1 for serial.
#define LIMIT_XL 2
#define LIMIT_YL 3
#define LIMIT_XR 4
#define LIMIT_YR 5
#define DIRN_XL 6
#define DIRN_YL 7
#define DIRN_XR 8
#define DIRN_YR 9
// 10-13 available for SPI
#define PULSE_XL 14
#define PULSE_YL 15
#define PULSE_XR 16
#define PULSE_YR 17
// 18-19 for I2C
#define NENABLE 20
#define RELAY 21
// 22/23 spare PWM or analogue (wire temp?)
#define LED 13
#define AN_OUT 14

Hardware::Hardware(){
  
  pinMode(LIMIT_XL, INPUT);
  pinMode(LIMIT_YL, INPUT);
  pinMode(LIMIT_XR, INPUT);
  pinMode(LIMIT_YR, INPUT);
  pinMode(DIRN_XL, OUTPUT);
  pinMode(DIRN_YL, OUTPUT);
  pinMode(DIRN_XR, OUTPUT);
  pinMode(DIRN_YR, OUTPUT);
  pinMode(PULSE_XL, OUTPUT);
  pinMode(PULSE_YL, OUTPUT);
  pinMode(PULSE_XR, OUTPUT);
  pinMode(PULSE_YR, OUTPUT);
  pinMode(NENABLE, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
  
  digitalWrite(DIRN_XL, LOW);
  digitalWrite(DIRN_YL, LOW);
  digitalWrite(DIRN_XR, LOW);
  digitalWrite(DIRN_YR ,LOW);
  digitalWrite(PULSE_XL,LOW);
  digitalWrite(PULSE_YL,LOW);
  digitalWrite(PULSE_XR,LOW);
  digitalWrite(PULSE_YR,LOW);
  digitalWrite(NENABLE,HIGH);
  digitalWrite(RELAY,LOW);
  digitalWrite(LED, LOW);
  _enabled = false;
  _limitsEnabled = true;
}

// Expect this to be called in an ISR
void Hardware::step(byte value){


  // Normally stop motors going past the limit switches but can turn this off
  // to drive away from the limit switches e.g. as part of homing an axis.
  if(_limitsEnabled){
    byte limits = getLimits();
    limits ^= 0x0F;  // Invert the limit pins.
    limits |= 0xF0;  // make sure we leave dirn alone.
    value &= limits; // clear any step inputs where endstop hit.
  }

  // Set direction bits
  digitalWrite(DIRN_XL, ((value & 0x80) != 0) ? HIGH : LOW);  
  digitalWrite(DIRN_YL, ((value & 0x40) != 0) ? HIGH : LOW);  
  digitalWrite(DIRN_XR, ((value & 0x20) != 0) ? HIGH : LOW);  
  digitalWrite(DIRN_YR, ((value & 0x10) != 0) ? LOW : HIGH); // swapped pending a bit of rewiring.

  // Set appropriate pulse bits
  digitalWrite(PULSE_XL, ((value & 0x08) != 0) ? HIGH : LOW);  
  digitalWrite(PULSE_YL, ((value & 0x04) != 0) ? HIGH : LOW);  
  digitalWrite(PULSE_XR, ((value & 0x02) != 0) ? HIGH : LOW);  
  digitalWrite(PULSE_YR, ((value & 0x01) != 0) ? HIGH : LOW);

//  digitalWrite(PULSE_XL, HIGH);  
//  digitalWrite(PULSE_YL, HIGH);  
//  digitalWrite(PULSE_XR, HIGH);  
//  digitalWrite(PULSE_YR, HIGH);

  delayMicroseconds(10);

  // And clear pulse bits
  digitalWrite(PULSE_XL, LOW);  
  digitalWrite(PULSE_YL, LOW);  
  digitalWrite(PULSE_XR, LOW);  
  digitalWrite(PULSE_YR, LOW);

}

void Hardware::setEnabled(bool enabled){
  _enabled = enabled;
  digitalWrite(NENABLE, enabled ? 0 : 1);
}

bool Hardware::isEnabled(){
  return _enabled;
}

// Gets the state of the limit switches, active high.
byte Hardware::getLimits(){
  byte val = (digitalRead(LIMIT_XL) == HIGH) ? 1 : 0;
  val <<= 1;
  val |= (digitalRead(LIMIT_YL) == HIGH) ? 1 : 0;
  val <<= 1;
  val |= (digitalRead(LIMIT_XR) == HIGH) ? 1 : 0;
  val <<= 1;
  val |= (digitalRead(LIMIT_YR) == HIGH) ? 1 : 0;
  return val;
}

void Hardware::enableHotWire(bool enabled){
  digitalWrite(RELAY, enabled ? HIGH : LOW);
}

void Hardware::enableStatusLed(bool enabled){
    digitalWrite(LED, enabled ? HIGH : LOW);
}

void Hardware::enableLimits(bool enabled){
  _limitsEnabled = enabled;
}
