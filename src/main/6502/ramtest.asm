    * = $f800 "Derpes"
    .for (var i = 0; i < 7; i++) {
      .for (var j = 0; j < 256; j++) {
         .byte j

      }
    }
    * = $ffc0 "Implementation"
#import "memorymap.asm"
#import "65c02.asm"

start:
    // Turn on led on port A pin 0
    lda #$01
    sta VIA1_DDRA
    lda #$01
    sta VIA1_PORTA

    jsr beepBuzzer

!:
    ldy #0
    {
    !:
        dey
        bne !-
    }
    dex
    stx VIA1_T1L
    jmp !-

beepBuzzer:
    lda #$80
    sta VIA1_DDRB

    // Beep buzzer
    ldx #255
    stx VIA1_T1L
    stz(VIA1_T1H)
    lda #$c0
    sta VIA1_ACR
    rts

ISR:
    rti

    * = $fffa "6502 vectors"
    .byte <ISR, >ISR
    .byte <start, >start
    .byte <ISR, >ISR
