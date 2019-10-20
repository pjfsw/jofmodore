# jofmodore
My 6502 SBC project

The bios.asm is the main code for the boot ROM

## Convert 6502 PRG binaries to Arduino loader format and upload it to Arduino Mega
`src/main/python/readprg.py target/bios.prg | src/main/python/programmer.py`
