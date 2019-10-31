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

    jsr console.init

    ldx #<msg
    ldy #>msg
    jsr console.println

    jsr printGraphicsId

    ldx #<cardInitMsg
    ldy #>cardInitMsg
    jsr console.println

    jsr card.sendCommand0
    jsr printCardStatus
    jsr card.sendCommand8 // Send next command in list and print status
    jsr printCardStatus
!:
    jsr beep

    jsr card.sendCommand1
    jsr printCardStatus
    lda CARD_R1
    bne !-

!:
    jmp !-

printGraphicsId: {
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
    rts
}

printCardStatus:
    .for (var i = 0; i < 2; i++) {
        lda CARD_CMD + i
        lsr(4)
        tay
        lda hexDigit,y
        sta cardStatus + i * 2
        lda CARD_CMD + i
        and #$0f
        tay
        lda hexDigit,y
        sta cardStatus + i * 2 + 1
    }

    ldx #<cardStatusMsg
    ldy #>cardStatusMsg
    jsr console.println
    rts

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
cardInitMsg:
    .text "INITIALIZING SDC"
    .byte 0

cardStatusMsg:
    .text "SDC STATUS: "
cardStatus:
    .text "    "
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