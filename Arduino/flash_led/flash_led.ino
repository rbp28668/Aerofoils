
int state = 0;

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);

}

void loop() {
  digitalWrite(LED_BUILTIN,state);
  state = state ^ 1;
  delay(250);

}
