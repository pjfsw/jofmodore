#importonce

// --------------------------------------
// Macros for 65C02 specific instructions
// --------------------------------------

// LDA (zp)
.macro ldazp(src) {
    .byte $b2
    .byte src
}

// STZ absolute
.macro stz(dest) {
    .byte $9c
    .byte <dest
    .byte >dest
}

.macro phx() {
    .byte $da
}

.macro phy() {
    .byte $5a
}

.macro plx() {
    .byte $fa
}

.macro ply() {
    .byte $7a
}

.macro deca() {
    .byte $3a
}

.macro inca() {
    .byte $1a
}

.macro inc16bit(addr) {
    inc addr
    bne !+
    inc addr+1
!:
}