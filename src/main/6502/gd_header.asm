#importonce
#import "memorymap.asm"

    .label GD_SPI_WRITE = $80
    .label GD_RAM_SPR = $30
    .label GD_RAM_SPRPAL = $38
    .label GD_RAM_SPRIMG = $40

    .label GD_REGISTER = $28
    .label GD_FRAME = $02
    .label GD_VBLANK = $03
    .label GD_SCROLL_Y = $06
    .label GD_SPR_DISABLE = $0a
    .label GD_BG_COLOR = $0e

.macro gd_select() {
    ldx #1
    jsr BIOS_SPI_selectDevice
}

.macro gd_deselect() {
    ldx #0
    jsr BIOS_SPI_selectDevice
}
