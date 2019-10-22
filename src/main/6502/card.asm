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
    .label CMD0 = $40
    .label CMD8 = $48

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
    rts
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
    lda #<command0
    sta SPI_DATA_PTR
    lda #>command0
    sta SPI_DATA_PTR+1
    jmp sendCommand
}

//
// SPI_DATA_PTR contains 5 byte sequence
// SPI_COUNT will be modified
sendCommand: {
    SPI_setupIndexFromDevice(CARD_DEVICE)
    stx SPI_PORT
!:
    jsr spi.readByteSendFF
    cmp #$FF
    bne !-

    // Send 6 bytes command
    lda #6
    sta SPI_COUNT
!:  {
        ldazp(SPI_DATA_PTR)
        SPI_writeAccumulator()
        inc16bit(SPI_DATA_PTR)
    !:
    }
    dec SPI_COUNT
    beq !+
    jmp !-

    // Get response size
!:
    lda #CARD_RETRY_COUNT
    sta SPI_COUNT
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

// Reset SDC
command0:
.byte CMD0, $00, $00, $00, $00, $95
// Check version
command8:
.byte CMD8, $00, $00, $01, $AA, $87

}