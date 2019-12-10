# jofmodore
My SBC project using Western Design Center 65C02 CPU

The bios.asm is the main code for the boot ROM

Memory map:
- `$E000-$FFFF` 8K ROM (1)
- `$D000-$D00F` 6522 VIA (SPI etc)
- `$C800-$C803` 6551 (2)
- `$8000-$83FF` 64x64 Screen Memory (1 KB) (3)
- `$0000-$7FFF` 32 KB RAM (4)

(1) Only `$F800-$FFFF` implemented in Arduino emulation

(2) Not yet implemented

(3) Not yet implemented

(4) Only `$0000-$07FF` implemented in Arduino emulation
  
## VIA pins

PortA:
- 0: Serial clock output (SPI, others)
- 1: SPI device 1 select (Gameduino)
- 2: SPI device 2 select (formerly SD card, currently not used)
- 3: SPI 128KB EEPROM/RAM select (Cartridge)
- 4: TBD
- 5: TBD
- 6: SPI MOSI
- 7: SPI MISO

PortB:
- 0: TBD
- 1: TBD
- 2: TBD
- 3: TBD
- 4: TBD
- 5: TBD
- 6: Cartridge load ready
- 7: Buzzer output
  
## Boot up sequence

Computer will boot up and wait for cartridge load ready to go high, then load the first 256
bytes into location `$0300`. It will then check the byte at `$0300` how many additional
256 byte pages to load and load these, then jump to location `$0301`.

## SPI Configuration

### Writing of SPI ROM (device 3):

TBD

### Wiring of SPI RAM (device 3):

An Arduino is used to read program data from a computer via serial port, then write it into an SPI
SRAM circuit, from which the Jofmodore boots a program. This simulates a ROM cartridge (see previous
section) but being programmable at the same time.

Parts: 

- [Microchip 23LC1024 128KB SPI SRAM](http://ww1.microchip.com/downloads/en/DeviceDoc/20005142C.pdf)
- [74HC245 Octal tranceiver](http://www.ti.com/lit/ds/symlink/sn74hc245.pdf)
- [74HC157 Quad 2-line to 1-line data selector](http://www.ti.com/lit/ds/symlink/sn74hc157.pdf)

`MISO`, `MOSI` and `CLK` are connected from 6522 to RAM via an 74HC245 octal tranceiver.
This is to prevent bus contention because the RAM is shared with the Arduino. 

An `/Enable Spi` signal from the Arduino is connected to the `/AB` input of the 74HC157. When the
signal is low, the 65C22 will have RAM access. When it is high, the Arduino will have RAM access.
Thus, 65C22 input signals are connected to the A-inputs of the 74HC157, and the Arduino input 
signals are connected to the B-inputs, as follows:
- 2A: 65C22 `RAM /CS` output
- 2B: Arduino `RAM /CS` output
- 2Y: RAM `/CS` input
- 4A: 65C22 `RAM /CS` output
- 4B: +5V
- 4Y: 74HC245 `/OE` input

The 4B=+5V means that the `/OE` is always high when the Arduino has RAM access. Thus, the octal
tranceiver disconnects the 65C22 `MISO`, `MOSI`, `CLK` from the RAM. When the the 65C22 has 
RAM access, it must detach itself from the SRAM whenever it is not actively accessing the RAM.

## Convert 6502 PRG binaries to Arduino loader format and upload it to Arduino Mega
Note that this function is now obsolete as the BIOS code which loads SPI cartridgs is stored*
in the Arduino Mega (later on in EEPROM). 

`src/main/python/readprg.py target/bios.prg | src/main/python/programmer.py`

The Python code will eventually be moved into its own project.
## Write 6502 PRG binaries to serial RAM or ROM
`hexdump -v -s 2 target/loadedprg.prg | src/main/python/programmer.py`

## Gameduino

The Gameduino interface will eventually be replaced with a native graphics adapter
available at address $8000-$83FF (200x150 pixels monochrome)

## SDC information

http://chlazza.nfshost.com/sdcardinfo.html

http://elm-chan.org/docs/mmc/mmc_e.html

https://stackoverflow.com/questions/8080718/sdhc-microsd-card-and-spi-initialization

https://openlabpro.com/guide/interfacing-microcontrollers-with-sd-card/

[FAT-16 information](https://www.analog.com/media/en/technical-documentation/application-notes/ee_329_rev1.pdf)