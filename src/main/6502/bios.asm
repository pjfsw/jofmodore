    * = $f800 "Libraries"

chunk_start:
chunk_spi:
#import "spi.asm"
    .print "SPI Chunk:       " + (*-chunk_spi)
chunk_cartridge:
#import "cartridge.asm"
    .print "Cartridge Chunk: " + (*-chunk_cartridge)
chunk_gameduino:
#import "gd.asm"
    .print "Gameduino Chunk: " + (*-chunk_gameduino)
chunk_sound:
#import "sound.asm"
    .print "Sound Chunk:     " + (*-chunk_sound)
    .print "Libraries TOTAL: " + (*-chunk_start)
chunk_bios:

.macro lsr(n) {
    .for(var i = 0;i < n;i++) {
        lsr
    }
}
start:
    nop
    jsr spi.init
    jsr console.init

    ldx #<msgWelcome
    ldy #>msgWelcome
    jsr console.println

    jsr printGraphicsId

    ldx #<msgWrite
    ldy #>msgWrite
    jsr console.println

    jsr cartridge.write256BytesToRAM

    ldx #<msgRead
    ldy #>msgRead
    jsr console.println

    jsr cartridge.readBootSector
    ldx #0
    ldy #4
    jsr console.println

    ldx #<msgDone
    ldy #>msgDone
    jsr console.println


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
    jmp console.println
}

beep: {
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
    jmp sound.quiet
}

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
msgWelcome:
    .text "JOFMODORE V0.01 INIT"
    .byte 0
msgWrite:
    .text "WRITING"
    .byte 0
msgRead:
    .text "READING"
    .byte 0
msgDone:
    .text "DONE"
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

.print "BIOS Chunk:      " + (* - chunk_bios)
.print "TOTAL BYTES:     " + (* - chunk_start)

    * = $fffc "6502 vectors"
    .byte <start, >start
    .byte $00,$01