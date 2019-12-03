#importonce

.pseudopc $0300 {

loadedprg:
    ldx #<loadedMsg
    ldy #>loadedMsg
    jsr console.println
!:
    jmp !-

loadedMsg:
    .text "THIS IS LOADED SOFTWARE!"
    .byte 0

.print "Size of loaded software: " + (*-loadedprg)
}

