// Default code from Arduino ROM

#import "memorymap.asm"

    * = $ffef "Implementation"
start:
    lda #$7f
    sta SPI_PORT
    lda #$7f        // all outputs except pin 7 which is MISO
    sta SPI_DDR
    jmp *
end:
    * = $fffc "6502 vectors"
    .byte <start, >start
