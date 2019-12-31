#importonce
#import "memorymap.asm"
#import "gd_header.asm"

.pseudopc $0300 {
    .byte (loadedPrgEnd-$300)/256

.macro lsr(n) {
    .for(var i = 0;i < n;i++) {
        lsr
    }
}

loadedprg:
    jmp prgStart
    rti             // In case of ISR
prgStart:
    ldx #<loadedMsg
    ldy #>loadedMsg
    jsr BIOS_Console_println

    ldx #<moreMsg
    ldy #>moreMsg
    jsr BIOS_Console_println

    ldx #1
    jsr BIOS_SPI_selectDevice
    {
        lda #GD_REGISTER
        jsr BIOS_SPI_writeByte
        lda #0
        jsr BIOS_SPI_writeByte
        jsr BIOS_SPI_readByte
        sta aNumber
    }
    ldx #0
    jsr BIOS_SPI_selectDevice

    lda aNumber
    lsr(4)
    tay
    lda hexDigit,y
    sta numberHighNibble
    lda aNumber
    and #$0f
    tay
    lda hexDigit,y
    sta numberLowNibble
    ldx #<numberMsg
    ldy #>numberMsg
    jsr BIOS_Console_println
!:

    jmp !-

hexDigit:
    .byte '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'

loadedMsg:
    .text "THIS IS LOADED SOFTWARE!"
    .byte 0
    .fill 2048,0
moreMsg:
    .text "SOME 2048 BYTES LATER!"
    .byte 0

aNumber:
    .byte $55
numberMsg:
    .text "A NUMBER IS "
numberHighNibble:
    .byte 1
numberLowNibble:
    .byte 1
    .byte 0
loadedPrgEnd:
}

