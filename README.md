# jofmodore
My SBC project using Western Design Center 65C02 CPU

The bios.asm is the main code for the boot ROM

Memory map:
- $E000-$FFFF ROM (1)
- $D000-$D00F 6522 VIA (SPI etc)
- $C800-$C803 6551 (2)
- $8000-$83FF 64x64 Screen Memory (1 KB) (3)
- $0000-$7FFF 32 KB RAM (4)

(1) Only $F100-$FFFF implemented in Arduino emulation

(2) Not yet implemented

(3) Not yet implemented

(4) Only $0000-$04FF implemented in Arduino emulation
  
## VIA pins

PortA:
- 0: Serial clock output (SPI, others)
- 1: SPI device 1 select (Gameduino)
- 2: SPI device 2 select
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

Computer will boot up and wait for cartridge load ready to go high, then load
the first XX KB of data from cartridge address 0 into address $0300 in RAM. Execution
will then jump to $0300.

## SPI Configuration

## Convert 6502 PRG binaries to Arduino loader format and upload it to Arduino Mega
`src/main/python/readprg.py target/bios.prg | src/main/python/programmer.py`

The Python code will eventually be moved into its own project.

## Gameduino

The gameduino interface will eventually be replaced with a native graphics adapter
available at address $8000-$83FF (200x150 pixels monochrome)

## References

The following web resources have been most helpful:

### SDC information

http://chlazza.nfshost.com/sdcardinfo.html

http://elm-chan.org/docs/mmc/mmc_e.html

https://stackoverflow.com/questions/8080718/sdhc-microsd-card-and-spi-initialization

https://openlabpro.com/guide/interfacing-microcontrollers-with-sd-card/

[FAT-16 information](https://www.analog.com/media/en/technical-documentation/application-notes/ee_329_rev1.pdf)