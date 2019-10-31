#define CLK 53
#define RESET 52
#define RWB 50
#define IO_CS 10
#define DEBUGPIN 11

#define ADDR_L PINA // pin 22-29 22=a0..29=a7
#define ADDR_L_DIR DDRA

#define ADDR_H PINC // pin 37-30 37=a8..30=a15
#define ADDR_H_DIR DDRC

#define DATAIN PINL // pin 49-42 49=d0..42=d7
#define DATAOUT PORTL
#define DATA_DIR DDRL

#define DIR_OUTPUT 255
#define DIR_INPUT 0


#define ROM_START 0xf000
#define ROM_SIZE 0x1000
#define RAM_SIZE 0x400

#define IO_START 0xA000
#define IO_END 0xA00F

#define RESET_LENGTH 6


#define MAX_ROW_LENGTH 256

char textbuf[MAX_ROW_LENGTH+1];
char data[MAX_ROW_LENGTH+1];

unsigned char rom[ROM_SIZE];
unsigned char ram[RAM_SIZE];

int led = 0;
int resetTimer = RESET_LENGTH;

void setup() {
  Serial.begin(230400);

  pinMode(RWB, INPUT);
  pinMode(RESET, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DEBUGPIN, OUTPUT);
  pinMode(IO_CS, OUTPUT);

  digitalWrite(RESET, 0);
  
  ADDR_L_DIR = DIR_INPUT;
  ADDR_H_DIR = DIR_INPUT;

   //ef ff a9 7f 8d 01 a0 a9 7f 8d 03 a0 4c f9 ff ef 
   rom[ROM_SIZE-17] = 0xa9;
   rom[ROM_SIZE-16] = 0x7f;
   rom[ROM_SIZE-15] = 0x8d;
   rom[ROM_SIZE-14] = 0x01;
   rom[ROM_SIZE-13] = 0xa0;
   rom[ROM_SIZE-12] = 0xa9;
   rom[ROM_SIZE-11] = 0x7f;
   rom[ROM_SIZE-10] = 0x8d;
   rom[ROM_SIZE- 9] = 0x03;
   rom[ROM_SIZE- 8] = 0xa0;
   rom[ROM_SIZE- 7] = 0x4c;
   rom[ROM_SIZE- 6] = 0xf9;
   rom[ROM_SIZE -5] = 0xff;   
   rom[ROM_SIZE- 4] = 0xef;
   rom[ROM_SIZE- 3] = 0xff;
}

void startTimer() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 2; //15624 / 8192;
  
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11)  ; // WORKING
  TIMSK1 |= (1 << OCIE1A);
}

void resetClockSequence() {
    digitalWrite(CLK, 1);
    resetTimer--;
    if (resetTimer == 0) {
      digitalWrite(RESET, 1);
    }
    digitalWrite(CLK, 0);  
}

void normalClockSequence() {
  unsigned int addr = ADDR_L | (ADDR_H << 8);

  if (addr >= 0xa000 && addr <= 0xafff) {
    digitalWrite(IO_CS, 1);
    digitalWrite(CLK, 1);
    digitalWrite(DEBUGPIN, 1);
  } else {
    digitalWrite(CLK, 1);
    digitalWrite(DEBUGPIN,0);
    
    if (digitalRead(RWB)) { // READ
      DATA_DIR = DIR_OUTPUT;
      DATAOUT = getByte(addr);
    } else { // WRITE
     setByte(addr, DATAIN);
    }
  }

  //delay(2);
  //digitalWrite(DEBUGPIN, ram[RAM_SIZE-1] % 2 == 0);
  digitalWrite(CLK, 0);   // toggle LED pin
  DATA_DIR = DIR_INPUT;
  digitalWrite(IO_CS, 0);
;
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  if (resetTimer > 0) {
    resetClockSequence();
  } else if (resetTimer == 0) {
    normalClockSequence();
  }
}



void readLine(char *inputBuffer) {
  int pos = 0;
  int finished = false;
  inputBuffer[0] = 0;
  do {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        finished = true;
      } else if (c > 0) {
        inputBuffer[pos++] = c;
        inputBuffer[pos] = 0;
      }
    } 
  } while (!finished && (pos < MAX_ROW_LENGTH));   
}

unsigned char getByte(unsigned int address) {
   if (address < RAM_SIZE) {
      return ram[address];
    } else if (address >= ROM_START) {
      return rom[address - ROM_START];
    } else {
      return 0xff;
    }
}

void setByte(unsigned int address, unsigned char theByte) {
  if (address < RAM_SIZE) {
    ram[address] = theByte;
  } else if (address >= ROM_START && address <= 0xFFFF) {
    rom[address - ROM_START] = theByte;
  }
}

int isDec(char c) {
  return c >= '0' && c <= '9';
}

int isHex(char c) {
  return c >= 'A' && c <= 'F';
}

int isValidHexString(char *s) {
  if (strlen(s) % 2 != 0) {
    return 0;
  }
  for (int i = 0; i < strlen(s); i++) {
    if (!isDec(s[i]) && !isHex(s[i]))  {
      return 0;
    }
  }
  return 1;
}

void loop() {
  readLine(textbuf);

  char command=0;
  unsigned int address=0;
  
  int parameters = sscanf(textbuf, "%c %x %s", &command, &address, data);
  if (command =='r' && parameters == 2) {
    unsigned char theByte = getByte(address);
    sprintf(textbuf, "%04X: %02X", address, getByte(address));
    Serial.println(textbuf);
  } else if (command == 'w' && parameters == 3) {
    if (isValidHexString(data)) {
      unsigned char toWrite = 0;
      char *datapos = data;
      for (int i = 0; i < strlen(data); i+=2) {
        if (sscanf(datapos, "%2hhx", &toWrite)) {
          unsigned int addr = address + i/2;
          setByte(addr, toWrite);
          sprintf(textbuf,"%04X=%02X ", addr, getByte(addr));
          Serial.print(textbuf);
          datapos +=2;
        }
      }
      Serial.println();
    } else {
      sprintf(textbuf, "%04X: Data must be an even length hex string", address);
      Serial.println(textbuf);
    }
  } else if (command == '0') {
    resetTimer = 0;
    digitalWrite(RESET, 0);
    Serial.println("Reset triggered");
  } else if (command == '1') {
    resetTimer = RESET_LENGTH;
    startTimer();
    Serial.println("Reset released");
  } else if (parameters == 1) {
    Serial.print("Unknown command ");
    Serial.println(command);
  } else {
    Serial.println("Parse error");
  }
  Serial.flush();

}
