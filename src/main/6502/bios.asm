    * = $f700 "Implementation"

#import "spi.asm"
#import "card.asm"
#import "gd.asm"
#import "sound.asm"

    * = * "BIOS"

.macro lsr(n) {
    .for(var i = 0;i < n;i++) {
        lsr
    }
}
start:
    jsr spi.init
    jsr card.init
    sta CARD_R1

    jsr beep
    jsr console.init

    ldx #<msg
    ldy #>msg
    jsr console.println

    lda CONSOLE_ID
    lsr(4)
    tay
    lda hexDigit,y
    sta graphicsIdHigh
    lda CONSOLE_ID
    and #$0f
    tay
    lda hexDigit,y
    sta graphicsIdLow
    ldx #<graphicsMsg
    ldy #>graphicsMsg
    jsr console.println

!:
    lda CARD_R1
    lsr(4)
    tay
    lda hexDigit,y
    sta cardStatusHigh
    lda CARD_R1
    and #$0f
    tay
    lda hexDigit,y
    sta cardStatusLow
    ldx #<cardStatusMsg
    ldy #>cardStatusMsg
    jsr console.println

    jsr sleep

    jsr card.sendCommand0
    sta CARD_R1

    jmp !-

beep:
    ldy #1
!:  {
        lda beeps,y
        jsr sound.beep

        ldx #20
    !:
        dex
        bne !-
    }
    dey
    bpl !-
    jsr sound.quiet
    rts

sleep:
    ldy #31
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
cardStatusMsg:
    .text "SDC STATUS: "
cardStatusHigh:
    .byte 0
cardStatusLow:
    .byte 0
    .byte 0
graphicsMsg:
    .text "GFX ID: "
graphicsIdHigh:
    .byte 0
graphicsIdLow:
    .byte 0
    .byte 0
hexDigit:
    .byte '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'

    * = $fffc "6502 vectors"
    .byte <start, >start
    .byte $00,$01