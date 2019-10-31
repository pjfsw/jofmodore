# jofmodore
My SBC project using Western Design Center 65C02 CPU

The bios.asm is the main code for the boot ROM

## Convert 6502 PRG binaries to Arduino loader format and upload it to Arduino Mega
`src/main/python/readprg.py target/bios.prg | src/main/python/programmer.py`

The Python code will eventually be moved into its own project.

## References

The following web resources have been most helpful:

### SDC information

http://chlazza.nfshost.com/sdcardinfo.html

http://elm-chan.org/docs/mmc/mmc_e.html

https://stackoverflow.com/questions/8080718/sdhc-microsd-card-and-spi-initialization

https://openlabpro.com/guide/interfacing-microcontrollers-with-sd-card/

[FAT-16 information](https://www.analog.com/media/en/technical-documentation/application-notes/ee_329_rev1.pdf)