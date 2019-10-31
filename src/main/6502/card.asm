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
    .label CMD1 = $40 | 1
    .label CMD8 = $40 | 8
    .label CMD17 = $40 | 17

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
    jmp sendCommand
}

sendCommand8: {
    lda #<command8
    sta SPI_DATA_PTR
    lda #>command8
    sta SPI_DATA_PTR+1
    jmp sendCommand
}

sendCommand1: {
    lda #<command1
    sta SPI_DATA_PTR
    lda #>command1
    sta SPI_DATA_PTR+1
    jmp sendCommand
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
    CARD_deselect()
    CARD_dummybyte()
    rts
}

// Read boot sector of disk
readBlock0: {
    lda #<command17
    sta SPI_DATA_PTR
    lda #>command17
    sta SPI_DATA_PTR+1
    jsr sendCommand
    beq !+
    // Initialization error, cannot read
    rts
!:
    SPI_setupIndexFromDevice(CARD_DEVICE)
    stx SPI_PORT

    lda #CARD_RETRY_COUNT
    sta SPI_COUNT
!:
    jsr spi.readByteSendFF

    cmp #READ_DATA_TOKEN
    beq !+
    cmp #$20
    bcs !-
    // Data token error
    rts
!:
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

    SPI_setupIndexFromDevice(CARD_DEVICE)
    jsr spi.readBytesSendFF // Read CRC

    rts
}

// Reset SDC
command0:
.byte CMD0, $00, $00, $00, $00, $95

// Check version
command8:
.byte CMD8, $00, $00, $01, $AA, $87

// INitialization
command1:
.byte CMD1, $00, $00, $00, $00, $00

// Read single block
command17:
.byte CMD17, $00, $00, $00, $00, $00

}