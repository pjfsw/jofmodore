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


#define ROM_START 0xf800
#define ROM_SIZE 0xf00
#define RAM_SIZE 0x800

#define IO_START 0xD000
#define IO_END 0xD00F

#define RESET_LENGTH 6


#define MAX_ROW_LENGTH 256

char textbuf[MAX_ROW_LENGTH+1];
char data[MAX_ROW_LENGTH+1];

// ************************
// BOOT LOADER WITH WRITE!!
// ************************
#if 0
unsigned char rom[] = {

  /* 0x0000 */
  0xa9, 0x7e, 0x8d, 0x01, 0xd0, 0xa9, 0x7f, 0x8d, 0x03, 0xd0, 0x60, 0xbd, 0xf8, 0xfa, 0x8d, 0x01, 
  0xd0, 0x8d, 0x00, 0x02, 0x09, 0x40, 0x8d, 0x01, 0x02, 0x60, 0xae, 0x01, 0x02, 0xac, 0x00, 0x02, 
  0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 
  0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 
  0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 
  0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 
  0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 
  0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 
  0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 
  0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x60, 0xae, 0x01, 0x02, 0xac, 0x00, 0x02, 0xb2, 
  0x01, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 
  0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 
  0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 
  0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 
  0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 
  0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 
  /* 0x0100 */
  0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 
  0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xe6, 0x01, 0xd0, 0x02, 0xe6, 0x02, 0xce, 
  0x02, 0x02, 0xf0, 0x03, 0x4c, 0x9f, 0xf8, 0x60, 0xae, 0x01, 0x02, 0xac, 0x00, 0x02, 0x9c, 0x02, 
  0x02, 0xb2, 0x01, 0xd0, 0x01, 0x60, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 
  0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 
  0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 
  0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 
  0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 
  0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 
  0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 
  0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xee, 0x02, 
  0x02, 0xe6, 0x01, 0xd0, 0x02, 0xe6, 0x02, 0x4c, 0x31, 0xf9, 0xae, 0x01, 0x02, 0xac, 0x00, 0x02, 
  0xa5, 0x01, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 
  0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 
  0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 
  0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 
  /* 0x0200 */
  0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 
  0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 
  0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 
  0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xce, 0x02, 0x02, 0xf0, 0x03, 0x4c, 
  0xc0, 0xf9, 0x60, 0xa9, 0x00, 0xa2, 0x80, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 
  0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 
  0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 
  0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 
  0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 
  0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x49, 0xff, 0x60, 0xa0, 0x00, 0xa9, 0x00, 0xa2, 0x80, 
  0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 
  0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 
  0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 
  0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 
  0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 
  0x49, 0xff, 0x91, 0x01, 0xc8, 0xd0, 0xa5, 0x60, 0x3e, 0x3c, 0x3a, 0x36, 0x20, 0x3b, 0xfb, 0xa2, 
  /* 0x0300 */
  0x03, 0x20, 0x0b, 0xf8, 0xa9, 0x03, 0x20, 0x1a, 0xf8, 0x20, 0x2b, 0xfb, 0x9c, 0x01, 0x00, 0xa9, 
  0x03, 0x85, 0x02, 0x20, 0x9a, 0xfa, 0xae, 0x00, 0x03, 0xf0, 0x0a, 0xe6, 0x02, 0xda, 0x20, 0x9a, 
  0xfa, 0xfa, 0xca, 0xd0, 0xf6, 0xa2, 0x00, 0x20, 0x0b, 0xf8, 0x60, 0xa9, 0x00, 0x20, 0x1a, 0xf8, 
  0xa9, 0x00, 0x20, 0x1a, 0xf8, 0xa9, 0x00, 0x20, 0x1a, 0xf8, 0x60, 0xa2, 0x03, 0x20, 0x0b, 0xf8, 
  0xa9, 0x01, 0x20, 0x1a, 0xf8, 0xa9, 0x40, 0x20, 0x1a, 0xf8, 0xa2, 0x00, 0x20, 0x0b, 0xf8, 0x60, 
  0x20, 0x3b, 0xfb, 0xa2, 0x03, 0x20, 0x0b, 0xf8, 0xa9, 0x02, 0x20, 0x1a, 0xf8, 0x20, 0x2b, 0xfb, 
  0xa9, 0x7d, 0x85, 0x01, 0xa9, 0xfb, 0x85, 0x02, 0xa2, 0x02, 0xa9, 0x00, 0x8d, 0x02, 0x02, 0xda, 
  0x20, 0x99, 0xf8, 0xfa, 0xca, 0xd0, 0xf3, 0xa2, 0x00, 0x20, 0x0b, 0xf8, 0x60, 0x01, 0xa2, 0x12, 
  0xa0, 0x03, 0x20, 0xe2, 0xff, 0xa2, 0x2b, 0xa0, 0x04, 0x20, 0xe2, 0xff, 0x4c, 0x0f, 0x03, 0x54, 
  0x48, 0x49, 0x53, 0x20, 0x49, 0x53, 0x20, 0x4c, 0x4f, 0x41, 0x44, 0x45, 0x44, 0x20, 0x53, 0x4f, 
  0x46, 0x54, 0x57, 0x41, 0x52, 0x45, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  /* 0x0400 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x53, 0x4f, 0x4d, 0x45, 0x20, 0x32, 0x35, 0x36, 
  0x20, 0x42, 0x59, 0x54, 0x45, 0x53, 0x20, 0x4c, 0x41, 0x54, 0x45, 0x52, 0x21, 0x00, 0xa2, 0x01, 
  0x20, 0x0b, 0xf8, 0xa9, 0x28, 0x20, 0x1a, 0xf8, 0xa9, 0x00, 0x20, 0x1a, 0xf8, 0x20, 0x43, 0xfa, 
  0x8d, 0x07, 0x02, 0xa2, 0x00, 0x20, 0x0b, 0xf8, 0xa9, 0x40, 0x8d, 0x03, 0x02, 0xa9, 0x09, 0x8d, 
  0x04, 0x02, 0xa9, 0x04, 0x8d, 0x05, 0x02, 0x9c, 0x06, 0x02, 0xa2, 0x01, 0x20, 0x0b, 0xf8, 0xa9, 
  0xa8, 0x20, 0x1a, 0xf8, 0xa9, 0x0a, 0x20, 0x1a, 0xf8, 0xa9, 0x01, 0x20, 0x1a, 0xf8, 0xa2, 0x00, 
  /* 0x0500 */
  0x20, 0x0b, 0xf8, 0x60, 0x86, 0x01, 0x84, 0x02, 0xa2, 0x01, 0x20, 0x0b, 0xf8, 0xad, 0x04, 0x02, 
  0x09, 0x80, 0x20, 0x1a, 0xf8, 0xad, 0x03, 0x02, 0x20, 0x1a, 0xf8, 0x20, 0x28, 0xf9, 0xa2, 0x00, 
  0x20, 0x0b, 0xf8, 0x18, 0xad, 0x03, 0x02, 0x69, 0x40, 0x8d, 0x03, 0x02, 0x90, 0x0d, 0xae, 0x04, 
  0x02, 0xe8, 0xe0, 0x10, 0xd0, 0x02, 0xa2, 0x00, 0x8e, 0x04, 0x02, 0x18, 0xad, 0x05, 0x02, 0x69, 
  0x08, 0x8d, 0x05, 0x02, 0x90, 0x08, 0xad, 0x06, 0x02, 0x49, 0x01, 0x8d, 0x06, 0x02, 0xa2, 0x01, 
  0x20, 0x0b, 0xf8, 0xad, 0x04, 0x02, 0x09, 0x80, 0x20, 0x1a, 0xf8, 0xad, 0x03, 0x02, 0x20, 0x1a, 
  0xf8, 0xa9, 0x32, 0x8d, 0x02, 0x02, 0xa9, 0x20, 0x85, 0x01, 0x20, 0xba, 0xf9, 0xa2, 0x00, 0x20, 
  0x0b, 0xf8, 0xa2, 0x01, 0x20, 0x0b, 0xf8, 0xa9, 0xa8, 0x20, 0x1a, 0xf8, 0xa9, 0x06, 0x20, 0x1a, 
  0xf8, 0xad, 0x05, 0x02, 0x20, 0x1a, 0xf8, 0xad, 0x06, 0x02, 0x20, 0x1a, 0xf8, 0xa2, 0x00, 0x20, 
  0x0b, 0xf8, 0x60, 0xa9, 0x80, 0x8d, 0x02, 0xd0, 0xa9, 0xff, 0x8d, 0x04, 0xd0, 0x9c, 0x05, 0xd0, 
  0xa9, 0xc0, 0x8d, 0x0b, 0xd0, 0xa2, 0x00, 0xea, 0xca, 0xd0, 0xfc, 0x9c, 0x0b, 0xd0, 0x2c, 0x00, 
  0xd0, 0x50, 0xfb, 0x20, 0x00, 0xf8, 0x20, 0xbe, 0xfc, 0x20, 0xd3, 0xfd, 0x20, 0xfc, 0xfd, 0xa2, 
  0x1c, 0xa0, 0xfe, 0x20, 0x04, 0xfd, 0x20, 0xfc, 0xfa, 0xa2, 0x21, 0xa0, 0xfe, 0x20, 0x04, 0xfd, 
  0x4c, 0x01, 0x03, 0xa2, 0x0c, 0xa0, 0xfe, 0x20, 0x04, 0xfd, 0xad, 0x07, 0x02, 0x4a, 0x4a, 0x4a, 
  0x4a, 0xa8, 0xb9, 0x2f, 0xfe, 0x8d, 0x2c, 0xfe, 0xad, 0x07, 0x02, 0x29, 0x0f, 0xa8, 0xb9, 0x2f, 
  0xfe, 0x8d, 0x2d, 0xfe, 0xa2, 0x24, 0xa0, 0xfe, 0x4c, 0x04, 0xfd, 0x40, 0xa2, 0x06, 0xa0, 0xfe, 
  /* 0x0600 */
  0x20, 0x04, 0xfd, 0x4c, 0x50, 0xfb, 0x57, 0x52, 0x49, 0x54, 0x45, 0x00, 0x4a, 0x4f, 0x46, 0x4d, 
  0x4f, 0x44, 0x4f, 0x52, 0x45, 0x20, 0x56, 0x30, 0x2e, 0x30, 0x31, 0x00, 0x4c, 0x4f, 0x41, 0x44, 
  0x00, 0x4f, 0x4b, 0x00, 0x47, 0x46, 0x58, 0x20, 0x49, 0x44, 0x3a, 0x20, 0x00, 0x00, 0x00, 0x30, 
  0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  /* 0x0700 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x4c, 0x04, 0xfd, 0x4c, 0x9a, 0xfa, 0x4c, 0x43, 0xfa, 0x4c, 0xba, 0xf9, 0x4c, 0x28, 
  0xf9, 0x4c, 0x99, 0xf8, 0x4c, 0x1a, 0xf8, 0x4c, 0x0b, 0xf8, 0xfb, 0xfd, 0x93, 0xfd, 0xfb, 0xfd, 
  
};
#endif

#if 1
// ************************
// BOOTLOADER WITHOUT WRITE
// ************************
unsigned char rom[] = {

  /* 0x0000 */
  0xa9, 0x7e, 0x8d, 0x01, 0xd0, 0xa9, 0x7f, 0x8d, 0x03, 0xd0, 0x60, 0xbd, 0xf8, 0xfa, 0x8d, 0x01, 
  0xd0, 0x8d, 0x00, 0x02, 0x09, 0x40, 0x8d, 0x01, 0x02, 0x60, 0xae, 0x01, 0x02, 0xac, 0x00, 0x02, 
  0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 
  0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 
  0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 
  0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 
  0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 
  0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 
  0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 
  0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x60, 0xae, 0x01, 0x02, 0xac, 0x00, 0x02, 0xb2, 
  0x01, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 
  0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 
  0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 
  0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 
  0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 
  0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 
  /* 0x0100 */
  0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 
  0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xe6, 0x01, 0xd0, 0x02, 0xe6, 0x02, 0xce, 
  0x02, 0x02, 0xf0, 0x03, 0x4c, 0x9f, 0xf8, 0x60, 0xae, 0x01, 0x02, 0xac, 0x00, 0x02, 0x9c, 0x02, 
  0x02, 0xb2, 0x01, 0xd0, 0x01, 0x60, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 
  0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 
  0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 
  0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 
  0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 
  0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 
  0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 
  0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xee, 0x02, 
  0x02, 0xe6, 0x01, 0xd0, 0x02, 0xe6, 0x02, 0x4c, 0x31, 0xf9, 0xae, 0x01, 0x02, 0xac, 0x00, 0x02, 
  0xa5, 0x01, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 
  0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 
  0x8c, 0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 
  0x01, 0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 
  /* 0x0200 */
  0xd0, 0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 
  0x2a, 0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 
  0x90, 0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x8c, 0x01, 0xd0, 0x2a, 0x90, 
  0x03, 0x8e, 0x01, 0xd0, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xce, 0x02, 0x02, 0xf0, 0x03, 0x4c, 
  0xc0, 0xf9, 0x60, 0xa9, 0x00, 0xa2, 0x80, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 
  0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 
  0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 
  0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 
  0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 
  0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0x49, 0xff, 0x60, 0xa0, 0x00, 0xa9, 0x00, 0xa2, 0x80, 
  0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 
  0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 
  0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 
  0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 
  0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 0xec, 0x01, 0xd0, 0x2a, 0xee, 0x01, 0xd0, 0xce, 0x01, 0xd0, 
  0x49, 0xff, 0x91, 0x01, 0xc8, 0xd0, 0xa5, 0x60, 0x3e, 0x3c, 0x3a, 0x36, 0x20, 0x3b, 0xfb, 0xa2, 
  /* 0x0300 */
  0x03, 0x20, 0x0b, 0xf8, 0xa9, 0x03, 0x20, 0x1a, 0xf8, 0x20, 0x2b, 0xfb, 0x9c, 0x01, 0x00, 0xa9, 
  0x03, 0x85, 0x02, 0x20, 0x9a, 0xfa, 0xae, 0x00, 0x03, 0xf0, 0x0a, 0xe6, 0x02, 0xda, 0x20, 0x9a, 
  0xfa, 0xfa, 0xca, 0xd0, 0xf6, 0xa2, 0x00, 0x20, 0x0b, 0xf8, 0x60, 0xa9, 0x00, 0x20, 0x1a, 0xf8, 
  0xa9, 0x00, 0x20, 0x1a, 0xf8, 0xa9, 0x00, 0x20, 0x1a, 0xf8, 0x60, 0xa2, 0x03, 0x20, 0x0b, 0xf8, 
  0xa9, 0x01, 0x20, 0x1a, 0xf8, 0xa9, 0x40, 0x20, 0x1a, 0xf8, 0xa2, 0x00, 0x20, 0x0b, 0xf8, 0x60, 
  0xa2, 0x01, 0x20, 0x0b, 0xf8, 0xa9, 0x28, 0x20, 0x1a, 0xf8, 0xa9, 0x00, 0x20, 0x1a, 0xf8, 0x20, 
  0x43, 0xfa, 0x8d, 0x07, 0x02, 0xa2, 0x00, 0x20, 0x0b, 0xf8, 0xa9, 0x40, 0x8d, 0x03, 0x02, 0xa9, 
  0x09, 0x8d, 0x04, 0x02, 0xa9, 0x04, 0x8d, 0x05, 0x02, 0x9c, 0x06, 0x02, 0xa2, 0x01, 0x20, 0x0b, 
  0xf8, 0xa9, 0xa8, 0x20, 0x1a, 0xf8, 0xa9, 0x0a, 0x20, 0x1a, 0xf8, 0xa9, 0x01, 0x20, 0x1a, 0xf8, 
  0xa2, 0x00, 0x20, 0x0b, 0xf8, 0x60, 0x86, 0x01, 0x84, 0x02, 0xa2, 0x01, 0x20, 0x0b, 0xf8, 0xad, 
  0x04, 0x02, 0x09, 0x80, 0x20, 0x1a, 0xf8, 0xad, 0x03, 0x02, 0x20, 0x1a, 0xf8, 0x20, 0x28, 0xf9, 
  0xa2, 0x00, 0x20, 0x0b, 0xf8, 0x18, 0xad, 0x03, 0x02, 0x69, 0x40, 0x8d, 0x03, 0x02, 0x90, 0x0d, 
  0xae, 0x04, 0x02, 0xe8, 0xe0, 0x10, 0xd0, 0x02, 0xa2, 0x00, 0x8e, 0x04, 0x02, 0x18, 0xad, 0x05, 
  0x02, 0x69, 0x08, 0x8d, 0x05, 0x02, 0x90, 0x08, 0xad, 0x06, 0x02, 0x49, 0x01, 0x8d, 0x06, 0x02, 
  0xa2, 0x01, 0x20, 0x0b, 0xf8, 0xad, 0x04, 0x02, 0x09, 0x80, 0x20, 0x1a, 0xf8, 0xad, 0x03, 0x02, 
  0x20, 0x1a, 0xf8, 0xa9, 0x32, 0x8d, 0x02, 0x02, 0xa9, 0x20, 0x85, 0x01, 0x20, 0xba, 0xf9, 0xa2, 
  /* 0x0400 */
  0x00, 0x20, 0x0b, 0xf8, 0xa2, 0x01, 0x20, 0x0b, 0xf8, 0xa9, 0xa8, 0x20, 0x1a, 0xf8, 0xa9, 0x06, 
  0x20, 0x1a, 0xf8, 0xad, 0x05, 0x02, 0x20, 0x1a, 0xf8, 0xad, 0x06, 0x02, 0x20, 0x1a, 0xf8, 0xa2, 
  0x00, 0x20, 0x0b, 0xf8, 0x60, 0x78, 0xa9, 0x80, 0x8d, 0x02, 0xd0, 0xa9, 0xff, 0x8d, 0x04, 0xd0, 
  0x9c, 0x05, 0xd0, 0xa9, 0xc0, 0x8d, 0x0b, 0xd0, 0xa2, 0x00, 0xea, 0xca, 0xd0, 0xfc, 0x9c, 0x0b, 
  0xd0, 0x2c, 0x00, 0xd0, 0x50, 0xfb, 0x20, 0x00, 0xf8, 0x20, 0x50, 0xfb, 0x20, 0x63, 0xfc, 0xa2, 
  0x9c, 0xa0, 0xfc, 0x20, 0x96, 0xfb, 0x20, 0xfc, 0xfa, 0xa2, 0xa1, 0xa0, 0xfc, 0x20, 0x96, 0xfb, 
  0x4c, 0x01, 0x03, 0xa2, 0x8c, 0xa0, 0xfc, 0x20, 0x96, 0xfb, 0xad, 0x07, 0x02, 0x4a, 0x4a, 0x4a, 
  0x4a, 0xa8, 0xb9, 0xaf, 0xfc, 0x8d, 0xac, 0xfc, 0xad, 0x07, 0x02, 0x29, 0x0f, 0xa8, 0xb9, 0xaf, 
  0xfc, 0x8d, 0xad, 0xfc, 0xa2, 0xa4, 0xa0, 0xfc, 0x4c, 0x96, 0xfb, 0x40, 0x4a, 0x4f, 0x46, 0x4d, 
  0x4f, 0x44, 0x4f, 0x52, 0x45, 0x20, 0x56, 0x30, 0x2e, 0x30, 0x31, 0x00, 0x4c, 0x4f, 0x41, 0x44, 
  0x00, 0x4f, 0x4b, 0x00, 0x47, 0x46, 0x58, 0x20, 0x49, 0x44, 0x3a, 0x20, 0x00, 0x00, 0x00, 0x30, 
  0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  /* 0x0500 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  /* 0x0600 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  /* 0x0700 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x4c, 0x96, 0xfb, 0x4c, 0x9a, 0xfa, 0x4c, 0x43, 0xfa, 0x4c, 0xba, 0xf9, 0x4c, 0x28, 
  0xf9, 0x4c, 0x99, 0xf8, 0x4c, 0x1a, 0xf8, 0x4c, 0x0b, 0xf8, 0x8b, 0xfc, 0x25, 0xfc, 0x04, 0x03,   
};

#endif

unsigned char ram[RAM_SIZE];

int led = 0;
int resetTimer = RESET_LENGTH;

void setup() {
  pinMode(RWB, INPUT);
  pinMode(RESET, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DEBUGPIN, OUTPUT);
  pinMode(IO_CS, OUTPUT);

  digitalWrite(RESET, 0);
  
  ADDR_L_DIR = DIR_INPUT;
  ADDR_H_DIR = DIR_INPUT;

  resetTimer = RESET_LENGTH;
  startTimer();

   
}

void startTimer() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 4;
  
  TCCR1B |= (1 << WGM12);
  TCCR1B |= ((1 << CS11) | (1 << CS10))  ; // WORKING
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

  if (addr >= IO_START && addr <= IO_END) {
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


void loop() {

}

