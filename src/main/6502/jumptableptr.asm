#importonce

#import "memorymap.asm"
#import "spi.asm"

    * = BIOS_SPI_selectDevice "SPI Select Device"
    jmp spi.selectDevice

    * = BIOS_SPI_writeByte "SPI Write Byte"
    jmp spi.writeByte

    * = BIOS_SPI_writeBytes "SPI Write Bytes"
    jmp spi.writeBytes

    * = BIOS_SPI_writeBytesUntilZero "SPI Write zero terminated byte array"
    jmp spi.writeBytesUntilZero

    * = BIOS_SPI_writeByteManyTimes "SPI Write byte many times"
    jmp spi.writeByteManyTimes

    * = BIOS_SPI_readByte  "SPI Read byte"
    jmp spi.readByte

    * = BIOS_SPI_readMemoryPage "SPI Read 256 bytes into memory"
    jmp spi.readMemoryPage

    * = BIOS_Console_println "SPI Read 256 bytes into memory"
    jmp console.println


