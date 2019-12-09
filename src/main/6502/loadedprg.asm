#importonce
#import "memorymap.asm"
.pseudopc $0301 {

loadedprg:
    ldx #<loadedMsg
    ldy #>loadedMsg
    jsr BIOS_Console_println

    ldx #<moreMsg
    ldy #>moreMsg
    jsr BIOS_Console_println

!:
    jmp !-

loadedMsg:
    .text "THIS IS LOADED SOFTWARE!"
    .byte 0
    .fill 256,0
moreMsg:
    .text "SOME 256 BYTES LATER!"
    .byte 0
}

