#define FULL_SPEED_PIN 2
#define SPEED_PIN 3
#define MHZ_1 7
#define MHZ_2 3
#define RESET 6
#define CLOCK 9

void cycle(long speed) {
  digitalWrite(CLOCK, 0);
  delayMicroseconds(speed);
  digitalWrite(CLOCK, 1);
  delayMicroseconds(speed);
}

void debug(long speed) {
  for (int i = 0; i < 7; i++) {
    cycle(speed);
  }
  digitalWrite(RESET,1);
  while (true) {
    cycle(speed);
  }
}

void fullspeed(int speed) {
    TCCR1A = ( (1 << COM1A0));   
    TCCR1B = ((1 << WGM12) | (1 << CS10));
    TIMSK1 = 0;
    OCR1A = speed;  
    delay(10);
    digitalWrite(RESET, 1); 
}

void setup() {
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, 0);
  pinMode(CLOCK, OUTPUT);
  
  if (!digitalRead(FULL_SPEED_PIN)) {
    if (digitalRead(SPEED_PIN)) {
      debug(50);
    } else {
      debug(500000L);
    }
  } else {
    if (digitalRead(SPEED_PIN)) {
      fullspeed(MHZ_2);
    } else {
      fullspeed(MHZ_1);
    }
  }
}

void loop() {
}
