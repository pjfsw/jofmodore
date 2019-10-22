#importonce

// -----------------------
// SD card  implementation
// -----------------------
//
// http://chlazza.nfshost.com/sdcardinfo.html
// http://elm-chan.org/docs/mmc/mmc_e.html
// https://stackoverflow.com/questions/8080718/sdhc-microsd-card-and-spi-initialization
// https://openlabpro.com/guide/interfacing-microcontrollers-with-sd-card/

#import "spi.asm"

.namespace card {
    * = * "SD Card"

    .label CARD_DEVICE = spi.SPI_DEVICE2
    .label CMD0 = %01000000

    .label CARD_RETRY_COUNT = 10

// Return R1 sesponse
init: {
    // Send >=75 clock pulses with no chip select and MOSI=1
    lda #spi.SPI_NO_DEVICE | spi.MOSI_MASK
    sta SPI_PORT

    ldx #80
!:
    inc SPI_PORT
    dec SPI_PORT
    dex
    bne !-

    jsr wait

    jmp sendCommand0

}

wait: {
    phy()

    ldy #0
!:
    dey
    bne !-

    ply()
    rts

}
.macro CARD_noArgument() {
    lda #0
    SPI_writeAccumulator()
    SPI_writeAccumulator()
    SPI_writeAccumulator()
    SPI_writeAccumulator()
}

.macro CARD_byte(byte) {
    lda #byte
    SPI_writeAccumulator()
}

.macro CARD_deselect() {
    lda #spi.SPI_NO_DEVICE
    sta SPI_PORT
}

.macro CARD_dummybyte() {
    CARD_byte($ff)
}


sendCommand0: {
    lda #CARD_RETRY_COUNT
    sta SPI_COUNT

    SPI_setupIndexFromDevice(CARD_DEVICE)
    stx SPI_PORT
!:
    jsr spi.readByteSendFF
    cmp #$FF
    bne !-

    CARD_byte(CMD0)
    CARD_noArgument()
    CARD_byte($95)   // CRC

!:
    jsr spi.readByteSendFF
    cmp #$80
    bcc !+        // bit 7 not set, exit

    dec SPI_COUNT // Retry a few times, otherwise return whatever status
    beq !+
    jmp !-

!:
    pha
    CARD_deselect()
    CARD_dummybyte()
    pla
    rts
}

}