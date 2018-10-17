/*
 * Sketch for testing stepper motor using TB6600
 */
int state = 0;
int count = 0;
int dirn = 0;
#define DIR 3
#define STEP 2
#define NENABLE 4

void setup() {
 
  pinMode(DIR,OUTPUT);
  pinMode(STEP,OUTPUT);
  pinMode(NENABLE,OUTPUT);

  digitalWrite(NENABLE, 0);
  digitalWrite(STEP, 0);
  digitalWrite(DIR,0);

}

void loop() {
  state = state ^ 1;

  if(count <= 0) {
    count = 10000;
    dirn = dirn ^ 1;
    digitalWrite(DIR,dirn);
  }

  step();
  --count;
}

void step() {
  digitalWrite(STEP,1);
  delayMicroseconds(1);
  digitalWrite(STEP,0);

  delayMicroseconds(400);  // 400uS max speed @12v & 4uStep
  //delayMicroseconds(50); // 50uS about as fast as it will go @12v & 16uStep

}

