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
    .label CMD0 = $40 | 0
    .label CMD8 = $40 | 8
    .label CMD17 = $40 | 17
    .label CMD55 = $40 | 55

    .label APP_CMD41 = $40 | 41

    .label CARD_RETRY_COUNT = 10

    .label READ_DATA_TOKEN = $FE

//
// Initialize card reader.
//
// Should be called before any other SPI operation due to the fact that the
// SDC is initialized based on clock pulses regardless of chip select
//
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

    // Waste a few cycles letting the SDC get ready
    // TODO: prettier delay routine somewhere
    phy()

    ldy #0
!:
    dey
    bne !-

    ply()

    rts
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

//
// Initialize card reader
//
sendCommand0: {
    lda #<command0
    sta SPI_DATA_PTR
    lda #>command0
    sta SPI_DATA_PTR+1
    jsr sendCommand
    CARD_deselect()
    CARD_dummybyte()
    rts
}

sendCommand8: {
    lda #<command8
    sta SPI_DATA_PTR
    lda #>command8
    sta SPI_DATA_PTR+1
    jsr sendCommand
    CARD_deselect()
    CARD_dummybyte()

    rts
}

sendAppCommand41: {
    lda #<command55
    sta SPI_DATA_PTR
    lda #>command55
    sta SPI_DATA_PTR+1
    jsr sendCommand
    lda #<appCommand41
    sta SPI_DATA_PTR
    lda #>appCommand41
    sta SPI_DATA_PTR+1
    jsr sendCommand
    CARD_deselect()
    rts
}


//
// Send a command using 6 byte sequence stored at the
// address pointed to by SPI_DATA_PTR
//
// The value SPI_COUNT will be modified
//
sendCommand: {
    // Store the command currently being sent
    ldazp(SPI_DATA_PTR)
    sta CARD_CMD

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
    // Store response
    sta CARD_R1
    rts
}

// Read boot sector of disk
readBootsector: {
    lda #<command17
    sta SPI_DATA_PTR
    lda #>command17
    sta SPI_DATA_PTR+1
    jsr sendCommand
    cmp #$0
    beq !+
    sta CARD_R1
    // Initialization error, cannot read
    rts
!:
    lda #$FF
    sta CARD_CMD

    SPI_setupIndexFromDevice(CARD_DEVICE)
    stx SPI_PORT

!:
    jsr spi.readByteSendFF

    sta CARD_R1
    cmp #READ_DATA_TOKEN
    beq !+
    cmp #$20
    bcs !-
    // Data token error
    rts
!:
    lda #$FE
    sta CARD_CMD

    // Begin data transfer
    lda #<CARD_BUFFER
    sta SPI_DATA_PTR
    lda #>CARD_BUFFER
    sta SPI_DATA_PTR+1

    SPI_setupIndexFromDevice(CARD_DEVICE)

    .for (var i = 0; i < 2; i++) {
        stz(SPI_COUNT) // 256 bytes
        jsr spi.readBytesSendFF
    }

    jsr spi.readByteSendFF // Read CRC

    CARD_deselect()
    CARD_dummybyte()

    rts
}

// Reset SDC
command0:
    .byte CMD0, $00, $00, $00, $00, $95

// Check version
command8:
    .byte CMD8, $00, $00, $01, $AA, $87

// Read single block
command17:
    .byte CMD17, $00, $00, $00, $00, $00

// Send APP Command initializer
command55:
    .byte CMD55, $00, $00, $00, $00, $00

//
// Application Commands - a CMD55 should precede
//
// Initialize High Capactity card, bit 30 = HC
appCommand41:
    .byte APP_CMD41, $40, $00, $00, $00, $00
}