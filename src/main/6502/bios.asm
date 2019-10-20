    * = $fb00 "Implementation"

#import "spi.asm"
#import "gd.asm"
#import "sound.asm"

    * = * "BIOS"

start:
    jsr spi.init
    jsr console.init


!:
    jsr beep

    ldx #<msg
    ldy #>msg
    jsr console.println

    jsr sleep

    jsr beep

    ldx #<msg2
    ldy #>msg2
    jsr console.println

    jsr sleep

    jsr beep

    ldx #<msg3
    ldy #>msg3
    jsr console.println

    jsr sleep

    jmp !-

beep:
    ldy #1
!:  {
        lda beeps,y
        jsr sound.beep

        ldx #60
    !:
        dex
        bne !-
    }
    dey
    bpl !-
    jsr sound.quiet
    rts

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

beeps:
    .byte 45,90
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