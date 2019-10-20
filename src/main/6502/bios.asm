    * = $fb00 "Implementation"

#import "spi.asm"
#import "gd.asm"

    * = * "BIOS"

start:
    jsr spi.init
    jsr console.init


!:
    ldx #<msg
    ldy #>msg
    jsr console.println

    jsr sleep

    ldx #<msg2
    ldy #>msg2
    jsr console.println

    jsr sleep

    ldx #<msg3
    ldy #>msg3
    jsr console.println

    jsr sleep

    jmp !-

sleep:
    ldy #63
!:  {
        ldx #255
    !:
        dex
        bne !-
    }
    dey
    bne !-
    rts

msg:
    .text "WELCOME TO JOFMODORE V0.01"
    .byte 0
msg2:
    .text "THERE IS NO BASIC PRESENT AND UNKNOWN BYTES FREE"
    .byte 0
msg3:
    .text "ALL YOUR BASE ARE BELONG TO US"
    .byte 0

    * = $fffc "6502 vectors"
    .byte <start, >start
    .byte $00,$01