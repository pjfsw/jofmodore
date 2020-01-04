//#define CLOCK 53
//#define RESET 51
#define WRITE 53
#define OE 52
#define DATA_DDR DDRA
#define DPORT PORTA
#define ADDRLO_DDR DDRL
#define ALO PORTL
#define ADDRHI_DDR DDRC
#define AHI PORTC

int v = 0;

//#define SPEED 1000

/*void cycle(int cycles) {
  for (int i = 0; i < cycles; i++) {
    digitalWrite(CLOCK, 1);
    digitalWrite(CLOCK, 0);
  }
 
}

void reset() {
  digitalWrite(RESET, 0);
  digitalWrite(RESET, 1);
*/


void setup() {
  pinMode(WRITE, OUTPUT);
  pinMode(OE, OUTPUT);
  digitalWrite(WRITE,1);
  digitalWrite(OE, 0);
  DATA_DDR = 255;
  ADDRLO_DDR = 255;
  ALO = 0;
}

void write(int data) {
  digitalWrite(OE, 1);
  delay(10);
  DPORT = data;
  delay(10);
  digitalWrite(WRITE, 0);
  delay(100);
  digitalWrite(WRITE, 1);
  delay(100);
  digitalWrite(OE, 0);
}

void loop() {
  write(0);
  delay(3000);
  write(255);
  delay(3000);
}
