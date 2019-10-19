    * = $fc00 "Implementation"

#import "spi.asm"

    .label GD_SPI_WRITE = $80
    .label GD_REGISTER = $28
    .label GD_SCROLL_Y = $06

    * = * "Bootup"

start:
    jsr spi.init

    ldx #1
    jsr spi.selectDevice
    lda #GD_REGISTER
    jsr spi.writeByte
    lda #$00
    jsr spi.writeByte
    jsr spi.readByte
    pha
    ldx #0
    jsr spi.selectDevice
    pla
    cmp #$6d
    bne !+

    lda #<(36*64)
    sta CONSOLE_POSITION
    lda #>(36*64)
    sta CONSOLE_POSITION+1
    lda #0
    sta CONSOLE_SCROLL
    sta CONSOLE_SCROLL+1

    jsr initGameduino


!:
    lda #<msg
    sta SPI_DATA_PTR
    lda #>msg
    sta SPI_DATA_PTR+1
    lda #msgend-msg
    sta SPI_COUNT
    jsr printLongMsg

    lda #<msg2
    sta SPI_DATA_PTR
    lda #>msg2
    sta SPI_DATA_PTR+1
    lda #msg2end-msg2
    sta SPI_COUNT
    jsr printLongMsg

    lda #<msg3
    sta SPI_DATA_PTR
    lda #>msg3
    sta SPI_DATA_PTR+1
    lda #msg3end-msg3
    sta SPI_COUNT
    jsr printLongMsg

    jmp !-

initGameduino:
    ldx #1
    jsr spi.selectDevice
    lda #GD_SPI_WRITE | GD_REGISTER
    jsr spi.writeByte
    lda #$0a
    jsr spi.writeByte
    lda #1
    jsr spi.writeByte
    ldx #0
    jsr spi.selectDevice
    rts

newLine:

printLongMsg:
    lda SPI_COUNT
    sta LINE_PAD
    ldx #1
    jsr spi.selectDevice

    lda CONSOLE_POSITION+1
    ora #GD_SPI_WRITE
    jsr spi.writeByte
    lda CONSOLE_POSITION
    jsr spi.writeByte

    jsr spi.writeBytes

    // Clear rest of  line
    sec
    lda #50
    sbc LINE_PAD
    sta SPI_COUNT
    lda #' '
    sta SPI_DATA_PTR
    jsr spi.writeByteManyTimes

    ldx #0
    jsr spi.selectDevice

    ldx #1
    jsr spi.selectDevice
    lda #GD_SPI_WRITE | GD_REGISTER
    jsr spi.writeByte
    lda #GD_SCROLL_Y
    jsr spi.writeByte
    lda CONSOLE_SCROLL
    jsr spi.writeByte
    lda CONSOLE_SCROLL+1
    jsr spi.writeByte

    ldx #0
    jsr spi.selectDevice

    clc
    lda CONSOLE_POSITION
    adc #$40
    sta CONSOLE_POSITION
    bcc !+
    {
        ldx CONSOLE_POSITION+1
        inx
        cpx #$10
        bne !+
        ldx #0
    !:
        stx CONSOLE_POSITION+1
    }
!:

    clc
    lda CONSOLE_SCROLL
    adc #$08
    sta CONSOLE_SCROLL
    bcc !+
    {
        lda CONSOLE_SCROLL+1
        eor #1
        sta CONSOLE_SCROLL+1
    }
!:
    rts


msg:
    .text "WELCOME TO JOFMODORE V0.01"
msgend:
msg2:
    .text "THERE IS NO BASIC PRESENT AND UNKNOWN BYTES FREE"
msg2end:
msg3:
    .text "ALL YOUR BASE ARE BELONG TO US"
msg3end:
    * = $fffc "6502 vectors"
    .byte <start, >start
    .byte $00,$01