    .const DDRA = $a003
    .const DDRB = $a002
    .const PORTA = $a001
    .const PORTB = $a000

    .const DEBUGPIN = $03ff
    .const SAVEX = $01
    .const SAVEY = $02

    * = $fd00
start:
    lda #$ff
    sta DDRA  // Set all outputs for port A
    sta DDRB // Set all outputs for port B

nextchar:
    ldx #10
    ldy textptr
    iny
    cpy #endtext-text
    bne !+
    ldy #0
!:
    sty textptr
    lda text,y
    sta haxxorline+1

refresh:
    ldy #7
drawline:
    stx SAVEX
    lda rows,y
    tax
haxxorline:
    lda font,y
    eor #$ff

    sty SAVEY
    ldy #255
    sty PORTA
    stx PORTB
    sta PORTA

    ldx SAVEX
    ldy SAVEY

    dey
    bmi chardone
    jmp drawline
chardone:
    dex
    bne refresh

    clc
    lda haxxorline+1
    adc #8
    cmp #<nomoretext
    bne !+
    lda #0
!:
    sta haxxorline+1

    jmp nextchar


rows:
    .byte 1,2,4,8,16,32,64,128

.align $100
text:
    .byte 0,0, 56, 56, 72,72, 32,32, 64,64, 72,72, 16,16, 72,72, 80,80,24,24, 88,88,96,96
    .byte 104,112,128,136,144,96, 0,8,0,8
// 16 = D
// 24 = E
// 32 = F
// 40 = H
// 48 = i
// 56 = j
// 64 = m
// 72 = o
// 80 = r
// 88 = !
// 96 = space
endtext:
textptr: .byte 0
.align $100

font:
    .byte %01111110
    .byte %10000001
    .byte %10100101
    .byte %10100101
    .byte %10000001
    .byte %10111101
    .byte %10000001  //0
    .byte %01111110

    .byte %01111110
    .byte %10000001
    .byte %10000001
    .byte %10111101
    .byte %10000001 //8
    .byte %10111101
    .byte %10000001
    .byte %01111110

    .byte %11111100
    .byte %11000110
    .byte %11000011
    .byte %11000011 //
    .byte %11000011
    .byte %11000011
    .byte %11000110
    .byte %11111110

    .byte %11111111
    .byte %11000000
    .byte %11000000
    .byte %11111100
    .byte %11000000
    .byte %11000000
    .byte %11000000
    .byte %11111111

    .byte %11111111
    .byte %11000000
    .byte %11000000
    .byte %11111100
    .byte %11000000
    .byte %11000000
    .byte %11000000
    .byte %11000000

    .byte %11000011
    .byte %11000011
    .byte %11000011
    .byte %11111111
    .byte %11111111
    .byte %11000011
    .byte %11000011
    .byte %11000011

    .byte %00111100
    .byte %00011000
    .byte %00011000
    .byte %00011000
    .byte %00011000
    .byte %00011000
    .byte %00011000
    .byte %00111100

    .byte %00000111
    .byte %00000011
    .byte %00000011
    .byte %00000011
    .byte %00000011
    .byte %11000011
    .byte %11000011
    .byte %01111110

    .byte %11000011
    .byte %11100111
    .byte %11111111
    .byte %11011011
    .byte %11000011
    .byte %11000011
    .byte %11000011
    .byte %11000011

    .byte %01111110
    .byte %11000011
    .byte %11000011
    .byte %11000011
    .byte %11000011
    .byte %11000011
    .byte %11000011
    .byte %01111110

    .byte %11111100
    .byte %11000110
    .byte %11000011
    .byte %11000110
    .byte %11111100
    .byte %11011000
    .byte %11001100
    .byte %11000110

    .byte %00011000
    .byte %00011000
    .byte %00011000
    .byte %00011000
    .byte %00011000
    .byte %00000000
    .byte %00011000
    .byte %00011000

    .byte %00000000
    .byte %00000000
    .byte %00000000
    .byte %00000000
    .byte %00000000
    .byte %00000000
    .byte %00000000
    .byte %00000000

    .byte %00000000
    .byte %00000000
    .byte %00000000
    .byte %00011000
    .byte %00011000
    .byte %00000000
    .byte %00000000
    .byte %00000000

    .byte %00000000
    .byte %00000000
    .byte %00111100
    .byte %00111100
    .byte %00111100
    .byte %00111100
    .byte %00000000
    .byte %00000000

    .byte %00000000
    .byte %00000000
    .byte %01100110
    .byte %00100100
    .byte %00000000
    .byte %00100100
    .byte %01100110
    .byte %00000000

    .byte %00000000
    .byte %01000100
    .byte %00101000
    .byte %00000000
    .byte %00000000
    .byte %00100100
    .byte %01000010
    .byte %00000000

    .byte %10000001
    .byte %00000000
    .byte %00000000
    .byte %00000000
    .byte %00000000
    .byte %00000000
    .byte %00000000
    .byte %10000001

nomoretext:
    * = $fffc
    .byte <start, >start
    .byte $00,$01