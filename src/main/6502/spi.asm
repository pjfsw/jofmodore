#importonce

// -----------------------
// 6522 SPI Implementation
// -----------------------

#import "memorymap.asm"
#import "65c02.asm"

.namespace spi {

// PA7:     MISO Read bit
// PA6:     MOSI Write bit
// PA1-PA5: DEVICE SELECT
// PA0:     SCLK Serial Clock

    .label SPI_NO_DEVICE = %00111110
    .label SPI_DEVICE1 =   %00111100
    .label SPI_DEVICE2 =   %00111010
    .label SPI_DEVICE3 =   %00110110
    .label MOSI_MASK =     %01000000

init: {
    lda #SPI_NO_DEVICE | MOSI_MASK
    sta SPI_PORT
    lda #$7f        // all outputs except pin 7 which is MISO
    sta SPI_DDR
    rts
}

// Device 1..n in X, 0 = no device selected
selectDevice: {
    lda deviceSelectTable,x
    sta SPI_PORT
    sta SPI_MOSI_L
    ora #MOSI_MASK
    sta SPI_MOSI_H
    rts
}

// A = Value to be written
// Y = MOSI=0 mask
// X = MOSI=1 mask
// To retrieve data, call rol
.macro @SPI_writeAccumulator() {
    .for (var i = 0; i < 8; i++) {
        sty SPI_PORT    // default MOSI = 0
        rol             // shift MSB into carry
        bcc !+          // if carry clear we are done (is 0)
        stx SPI_PORT    // carry set, MOSI = 1
    !:
        inc SPI_PORT    // raise clock
        dec SPI_PORT    // clear clock
    }
}

// Write on byte to SPI
// Device must be previously selected
// Byte in A
writeByte: {
    ldx SPI_MOSI_H
    ldy SPI_MOSI_L

    SPI_writeAccumulator()

    rts
}

//
// 16-bit Address to bytes in SPI_DATA_PTR
// Number of bytes to write in SPI_COUNT
// X,Y,A are not preserved after call.
//
writeBytes: {
    ldx SPI_MOSI_H
    ldy SPI_MOSI_L
!:
    ldazp(SPI_DATA_PTR)

    SPI_writeAccumulator()

    {

        inc SPI_DATA_PTR
        bne !+
        inc SPI_DATA_PTR+1
    !:
    }
    dec SPI_COUNT
    beq !+
    jmp !-
!:
    rts
}

//
// 16-bit Address to bytes ending with 0 in SPI_DATA_PTR
// After call, SPI_COUNT contains length
// X,Y,A are not preserved after call.
//

writeBytesUntilZero: {
    ldx SPI_MOSI_H
    ldy SPI_MOSI_L
    stz(SPI_COUNT)

!:  {
        ldazp(SPI_DATA_PTR)
        bne !+
        rts

    !:  {
            SPI_writeAccumulator()
            inc SPI_COUNT
            inc SPI_DATA_PTR
            bne !+
            inc SPI_DATA_PTR+1
        !:
        }
    }

    jmp !-
}


//
// 8-bit value to write in SPI_DATA_PTR
// Number of bytes to write in SPI_COUNT
// X,Y,A are not preserved after call.
//
writeByteManyTimes: {
    ldx SPI_MOSI_H
    ldy SPI_MOSI_L

!:
    lda SPI_DATA_PTR

    SPI_writeAccumulator()

    dec SPI_COUNT
    beq !+
    jmp !-
!:
    rts

}

.macro SPI_readByteIntoA() {
    lda #0
    ldx #$80

    .for (var i = 0;i < 8; i++) {
        cpx SPI_PORT
        rol
        inc SPI_PORT
        dec SPI_PORT
    }
    eor #$ff
}

// Read a byte from SPI
// Device must be previously selected
// Returns byte in A
// Y is not guaranteed to be preserved
readByte: {
    SPI_readByteIntoA()

    rts
}

// Perform a 256 byte read into RAM buffer
// Setup SPI port before call with whatever parameters needed i.e.
// the proper CS and the value of MOSI
//
// Target address in SPI_DATA_PTR
readMemoryPage: {
    ldy #0
!:
    SPI_readByteIntoA()
    sta (SPI_DATA_PTR),y
    iny
    bne !-
    rts
}

deviceSelectTable:
    .byte SPI_NO_DEVICE, SPI_DEVICE1, SPI_DEVICE2, SPI_DEVICE3


} // namespac