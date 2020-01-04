 const PROGMEM unsigned char data[] = {
  
};

#define ANALOG_OFFSET 14

int dataPins[] = {
  ANALOG_OFFSET + 0,  // IO0
  ANALOG_OFFSET + 1,  // IO1
  ANALOG_OFFSET + 2,  // IO2
  2,                  // IO3
  3,                  // IO4
  4,                  // IO5
  5,                  // IO6
  6                   // IO7
  };

#define OE 7
#define WE 8


#define LATCH (4 + ANALOG_OFFSET)
#define CLOCK (5 + ANALOG_OFFSET)
#define SER (3 + ANALOG_OFFSET)

void setAddress(unsigned int address) {
  digitalWrite(OE,1);
  digitalWrite(WE,1);
  digitalWrite(LATCH,0);
  digitalWrite(CLOCK,0);
  shiftOut(SER, CLOCK, MSBFIRST, address/256);    
  shiftOut(SER, CLOCK, MSBFIRST, address & 0xFF);    
  digitalWrite(LATCH,1);
  digitalWrite(LATCH,0);  
  delay(1);
}

int readByte() {
  datapins(INPUT);
  digitalWrite(WE,1);
  digitalWrite(OE,0);
  delay(1);
  int value = 0;
  for (int i = 0; i < 8; i++) {
    value |= digitalRead(dataPins[i]) << i; 
  }
  digitalWrite(OE,1);
  return value;
}

void printOkOrFail(int error) {
      if (error) {
      Serial.print("FAIL");
    } else {
      Serial.print("OK");
    }
}
void readData(int bytesToRead) {
  char out[5];
  int error = 0;
  for (int i = 0; i < bytesToRead; i++) {
    if (i % 16 == 0) {
      if (i > 0) {
        printOkOrFail(error);
      }
      error = 0;
      Serial.println();
      sprintf(out, "%04x ", i);
      Serial.print(out);
    }
    setAddress(i);
    int value = readByte();
    if (value != pgm_read_byte_near(data + i)) {
      error = 1;
    }
    sprintf(out, "%02x ", value);
    Serial.print(out);
  }
  printOkOrFail(error);
  Serial.println();
  Serial.flush();
}

void datapins(int direction) {
  for (int i = 0; i < 8; i++) {
    pinMode(dataPins[i], direction);
  }
}

void writeByte(unsigned char value) {
  digitalWrite(OE, 1);
  delay(1);
  datapins(OUTPUT);
  for (int i = 0; i < 8; i++) {
    digitalWrite(dataPins[i], (value & (1 << i)) ? 1 : 0);
  } 
  delay(1);
  digitalWrite(WE,0);
  delay(1);
  digitalWrite(WE,1);
  datapins(INPUT);
  delay(4);
}

void saveData() {
  char buf[100];
  for (int i = 0 ; i < sizeof(data); i++) {
    if (i % 256 == 0) {
      sprintf(buf, "Writing %04x-%04X", i, i+255);
      Serial.println(buf);
      Serial.flush();
    }
    setAddress(i);
    writeByte(pgm_read_byte_near(data + i));
  }
}

void setOutput(int pin, int value) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, value);
}

void setup() {
  Serial.begin(57600);
  datapins(INPUT);
  setOutput(OE, 1);
  setOutput(WE, 1);
  setOutput(LATCH, 0);
  setOutput(CLOCK, 0);
  setOutput(SER, 0);
  saveData();
  delay(10);
}

int i = 0;
void loop() {
  readData(sizeof(data));  
  delay(5000);
}
