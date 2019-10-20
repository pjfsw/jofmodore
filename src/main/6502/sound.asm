#importonce

//
// One channel simple sound interface
//
.namespace sound {

    .label SOUND_DDR = VIA1_DDRB
    .label SOUND_PORT = VIA1_PORTB
    .label SOUND_ACR = VIA1_ACR
    .label SOUND_T1L = VIA1_T1L
    .label SOUND_T1H = VIA1_T1H

init: {
    lda SOUND_DDR
    ora #$80 // PB7 = sound
    sta SOUND_DDR
    rts
}
//
// Frequency value in A (low)
beep: {
    sta VIA1_T1L
    stz(VIA1_T1H)
    lda #$c0
    sta VIA1_ACR
    rts
}

quiet: {
    lda #$00
    sta VIA1_ACR
    rts
}
}