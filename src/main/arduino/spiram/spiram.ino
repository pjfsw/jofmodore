#include <SPI.h>

#define DATA_SIZE 256

#define RAM_READ_MODE  0x03
#define RAM_WRITE_MODE 0x02
#define RAM_RDMR       0x05
#define RAM_WRMR       0x01

// DON'T USE PIN 10-13 FOR SOFTWARE
#define MISO_PIN         2
#define MOSI_PIN         3
#define CLK_PIN          4
#define CSB_PIN          5
#define ENABLE_SPIB_PIN  6
#define RESETB_PIN       7


#define SEQUENTIAL_MODE 0b01000000

#define MAX_ROW_LENGTH 256

char textbuf[MAX_ROW_LENGTH+1];

unsigned char data[DATA_SIZE];
unsigned char verify[DATA_SIZE];

unsigned int dataPos = 0;
unsigned long ramAddress = 0;

typedef enum {
  OK,
  FLUSH,
  WRITE_ERROR,
  VERIFY_ERROR,
  UNKNOWN_ERROR
} Result;

void spiBegin() {
  digitalWrite(CSB_PIN, LOW);
  //SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
}

void spiEnd() {
  //SPI.endTransaction();
  digitalWrite(CSB_PIN,HIGH);
}

int spiTransfer(int value) {
  int input = 0;
  for (int i = 7; i >= 0; i--) {
    digitalWrite(MOSI_PIN, (value & (1 << i)) ? 1 : 0);
    digitalWrite(CLK_PIN,1);
    digitalWrite(CLK_PIN,0);
    input |=  digitalRead(MISO_PIN) << i;
  }
  return input;
}

void spiAddress(long addressInRam) {
  spiTransfer(addressInRam >> 16);
  spiTransfer((addressInRam >> 8) & 0xFF);
  spiTransfer(addressInRam & 0xFF);
}
void storeSpiData(long addressInRam, int numberOfBytes) {
  
  // Store data from address 0 onwards
  spiBegin();
  spiTransfer(RAM_WRITE_MODE);
  spiAddress(addressInRam);
  for (int i = 0; i  < numberOfBytes; i++) {
    spiTransfer(data[i]);
  }
  spiEnd();
}

Result verifySpiData(long addressInRam, int numberOfBytes) {

  spiBegin();
  spiTransfer(RAM_READ_MODE);
  spiAddress(addressInRam);
  for (int i = 0; i < numberOfBytes; i++) {
    verify[i] = spiTransfer(0);
  }
  spiEnd();
  Result result = OK;
  for (int i = 0; i < numberOfBytes; i++) {
    if (verify[i] != data[i]) {
      result = VERIFY_ERROR;
    }
  }
  return result;
}

void boot() {
  digitalWrite(CSB_PIN, 1);
  pinMode(CLK_PIN, INPUT);
  pinMode(MOSI_PIN, INPUT);
  digitalWrite(ENABLE_SPIB_PIN, 0); 
  digitalWrite(RESETB_PIN, 1);
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

int isDec(char c) {
  return c >= '0' && c <= '9';
}

int isHex(char c) {
  return c >= 'a' && c <= 'f';
}

int isValidHexString(char *s) {
  if ((strlen(s) % 3) != 2) {
    return 0;
  }
  for (int i = 0; i < strlen(s); i++) {
    if (s[i] != 0x20 && !isDec(s[i]) && !isHex(s[i]))  {
      return 0;
    }
  }
  return 1;
}

Result writeSpiData() {
  if (dataPos == 0) {
    return OK;
  }
  storeSpiData(ramAddress, dataPos);
  Result result = verifySpiData(ramAddress, dataPos);
  ramAddress += dataPos;
  dataPos = 0;
  return result;
}

Result updateBuffer(int value) {
  data[dataPos] = value;
  dataPos++;
  Result result = OK;
  if (dataPos >= DATA_SIZE) {
    result = writeSpiData();
  }
  
  return result;
}

void writeResult(Result result) {
  switch (result) {
    case OK:
      Serial.println("OK");
      break;
    case FLUSH:
      Serial.println("OK (Flush)");
      break;
    case WRITE_ERROR:
      Serial.println("SPI Write error");
      break;
    case VERIFY_ERROR:
      Serial.println("SPI Verify error");
      break;
    case UNKNOWN_ERROR:
      Serial.println("Unknown error");
      break;
  }
}

Result storeHexString(char *hexchars) {
    int toWrite = 0;
    Result result = OK;
    char *datapos = hexchars;
    for (int i = 0; i < strlen(hexchars); i+=3) {
      if (sscanf(datapos, "%x", &toWrite)) {
        char derpes[4];
        sprintf(derpes, "%02x ", toWrite);
        Serial.write(derpes);
        Result tmpResult = updateBuffer(toWrite);
        if (tmpResult != OK) {
          result = tmpResult;
        }
        datapos +=3;
      }
    }
    return result;
}

void readFromSerial() {
  readLine(textbuf);

  char command=0;
  long tmpAddress;
  int dummy;
  int results;
  
  if (2 == sscanf(textbuf, "a %lx", &tmpAddress)) {
    writeResult(writeSpiData());
    ramAddress = tmpAddress;
  } else if (strncmp("b", textbuf, strlen("b")) == 0) {
    Result result = writeSpiData();
    if (result == OK) {
      boot();
      Serial.println("Booting");
    } else {
      writeResult(result); 
    }
    // Scan hexdump data
  } else if ((results = sscanf(textbuf, "%07x %x", &dummy, &dummy)) > 0) {
    if (results == 2) {
      char *hexchars = textbuf;
      while (*hexchars != 0x20  && *hexchars != 0x00 && *hexchars != 0x0d) {
        *hexchars++;
      }
      if (*hexchars == 0x00 || *hexchars == 0x0d) {
        Serial.println("Empty line");
      } else {
        hexchars++;
        if (isValidHexString(hexchars)) {
          writeResult(storeHexString(hexchars));
        } else {
          Serial.println(hexchars);
        }
      }
    } else {
      Serial.println("Ignoring empty line");
    }
  } else {
    Serial.println("Parse error");
  }
  Serial.flush();
  
}

void setup() {
  pinMode(CSB_PIN, OUTPUT);
  digitalWrite(CSB_PIN, 1);
  pinMode(RESETB_PIN, OUTPUT);
  digitalWrite(RESETB_PIN, 0);
  pinMode(ENABLE_SPIB_PIN, OUTPUT);
  digitalWrite(ENABLE_SPIB_PIN, 1);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);

  spiBegin();
  spiTransfer(RAM_WRMR);
  spiTransfer(SEQUENTIAL_MODE);
  spiEnd();

  Serial.begin(230400);
  Serial.println("SPI RAM/ROM loader.");
  Serial.println("ready");
}

void loop() {
  readFromSerial(); 
}
