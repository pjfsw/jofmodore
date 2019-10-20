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

