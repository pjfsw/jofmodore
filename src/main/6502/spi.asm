#importonce

// -----------------------
// 6522 SPI Implementation
// -----------------------

#import "memorymap.asm"
#import "65c02.asm"

.namespace spi {
    * = * "SPI"

// PA7:     MISO Read bit
// PA6:     MOSI Write bit
// PA1-PA5: DEVICE SELECT
// PA0:     SCLK Serial Clock

    .label SPI_DDR = VIA1_DDRA
    .label SPI_PORT = VIA1_PORTA
    .label SPI_NO_DEVICE = %00111110
    .label SPI_DEVICE1 =   %00111100
    .label MOSI_MASK =     %01000000

init: {
    lda #$7f        // all outputs except pin 7 which is MISO
    sta SPI_DDR
    lda #SPI_NO_DEVICE
    sta SPI_PORT
    rts
}

// Device 1..n in X, 0 = no device selected
selectDevice: {
    lda deviceSelectTable,x
    sta SPI_PORT
    sta SPI_MOSI0
    ora #MOSI_MASK
    sta SPI_MOSI1
    rts
}


// A,X, Y must be initialized
.macro writeAccumulator() {
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
    ldx SPI_MOSI1
    ldy SPI_MOSI0

    writeAccumulator()

    rts
}

//
// 16-bit Address to bytes in SPI_DATA_PTR
// Number of bytes to write in SPI_COUNT
// X,Y,A are not preserved after call.
//
writeBytes: {
    ldx SPI_MOSI1
    ldy SPI_MOSI0
!:
    ldazp(SPI_DATA_PTR)

    writeAccumulator()

    dec SPI_COUNT
    beq !+
    {

        inc SPI_DATA_PTR
        bne !+
        inc SPI_DATA_PTR+1
    !:
    }
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
    ldx SPI_MOSI1
    ldy SPI_MOSI0
    stz(SPI_COUNT)

!:  {
        ldazp(SPI_DATA_PTR)
        bne !+
        rts

    !:  {
            writeAccumulator()
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
    ldx SPI_MOSI1
    ldy SPI_MOSI0

!:
    lda SPI_DATA_PTR

    writeAccumulator()

    dec SPI_COUNT
    beq !+
    jmp !-
!:
    rts

}

// Read a byte from SPI
// Device must be previously selected
// Returns byte in A
readByte: {
    lda #0
   .for (var i = 0;i < 8; i++) {
        clc
        bit SPI_PORT
        bpl !+
        sec
    !:
        rol
        inc SPI_PORT
        dec SPI_PORT
    }

    rts
}

deviceSelectTable:
    .byte SPI_NO_DEVICE, SPI_DEVICE1


} // namespac