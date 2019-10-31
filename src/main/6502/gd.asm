#importonce

// -----------------------------------
// Gameduino implementation of console
// -----------------------------------

    .label GD_SPI_WRITE = $80
    .label GD_REGISTER = $28
    .label GD_SCROLL_Y = $06
    .label GD_SPR_DISABLE = $0a
    .label GD_BG_COLOR = $0e

.macro gd_select() {
    ldx #1
    jsr spi.selectDevice
}

.macro gd_deselect() {
    ldx #0
    jsr spi.selectDevice
}

.namespace  console {
    * = * "Gameduino"

init: {
    gd_select()
    {
        lda #GD_REGISTER
        jsr spi.writeByte
        lda #0
        jsr spi.writeByte
        jsr spi.readByte
        sta CONSOLE_ID
    } gd_deselect()

    lda #<(37*64)
    sta CONSOLE_POSITION
    lda #>(37*64)
    sta CONSOLE_POSITION+1
    lda #4
    sta CONSOLE_SCROLL
    stz(CONSOLE_SCROLL+1)

    // Disable sprites
    gd_select()
    {
        lda #GD_SPI_WRITE | GD_REGISTER
        jsr spi.writeByte
        lda #GD_SPR_DISABLE
        jsr spi.writeByte
        lda #1
        jsr spi.writeByte
    } gd_deselect()

    rts
}

// x/y high low msg, null terminated
println: {
    stx SPI_DATA_PTR
    sty SPI_DATA_PTR+1

    // Write text at current position
    gd_select()
    {
        lda CONSOLE_POSITION+1
        ora #GD_SPI_WRITE
        jsr spi.writeByte
        lda CONSOLE_POSITION
        jsr spi.writeByte

        jsr spi.writeBytesUntilZero
    } gd_deselect()

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

    gd_select()
    {
        lda CONSOLE_POSITION+1
        ora #GD_SPI_WRITE
        jsr spi.writeByte
        lda CONSOLE_POSITION
        jsr spi.writeByte

        // Clear next  line
        lda #50
        sta SPI_COUNT
        lda #' '
        sta SPI_DATA_PTR
        jsr spi.writeByteManyTimes
    } gd_deselect()

    gd_select()
    {
        lda #GD_SPI_WRITE | GD_REGISTER
        jsr spi.writeByte
        lda #GD_SCROLL_Y
        jsr spi.writeByte
        lda CONSOLE_SCROLL
        jsr spi.writeByte
        lda CONSOLE_SCROLL+1
        jsr spi.writeByte
    } gd_deselect()

    rts

}
}
