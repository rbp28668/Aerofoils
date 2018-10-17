/*
 * Sketch for testing stepper motor using DRV8255
 */
int state = 0;

#define NFAULT 4
#define DIR 5
#define STEP 6
#define NSLEEP 7
#define NRESET 8
#define M2 9
#define M1 10
#define M0 11
#define NENABLE 12

void setup() {
  pinMode(NFAULT,INPUT);

  pinMode(DIR,OUTPUT);
  pinMode(STEP,OUTPUT);
  pinMode(NSLEEP,OUTPUT);
  pinMode(NRESET,OUTPUT);
  pinMode(M2,OUTPUT);
  pinMode(M1,OUTPUT);
  pinMode(M0,OUTPUT);
  pinMode(NENABLE,OUTPUT);

  digitalWrite(NENABLE, 0);
  digitalWrite(NRESET, 1);
  digitalWrite(NSLEEP, 1);

  // full step mode.
  digitalWrite(M0,1);
  digitalWrite(M1,0);
  digitalWrite(M2,1);

  // some direction!
  digitalWrite(DIR,0);

}

void loop() {
  state = state ^ 1;

  if(digitalRead(NFAULT) == LOW) {
    digitalWrite(NENABLE,HIGH);
    digitalWrite(LED_BUILTIN,1);
  } else {
    digitalWrite(NENABLE, LOW);
    digitalWrite(LED_BUILTIN,0);
  }
  digitalWrite(STEP,1);
  delayMicroseconds(2);
  digitalWrite(STEP,0);

  //delay(1);
  delayMicroseconds(100);

}
