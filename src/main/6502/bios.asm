    * = $f800 "Implementation"

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
chunk_bios:

.macro lsr(n) {
    .for(var i = 0;i < n;i++) {
        lsr
    }
}

// The following code must NOT use the stack or RAM
// because it should be possible to execute before
// any RAM is installed, for testing purposes

.macro ROM_Only_Sequence() {
    // Init port B, bit 7 is sound output, bit 6 is cartridge ready bit
    lda #$80
    sta VIA1_DDRB

    // Beep buzzer
    lda #255
    sta VIA1_T1L
    stz(VIA1_T1H)
    lda #$c0
    sta VIA1_ACR

    // Delay a bit
    ldx #0
!:
    nop
    dex
    bne !-

    // Stop beeping
    stz(VIA1_ACR)

    // Wait for cartridge ready bit set
!:
    bit VIA1_PORTB
    bvc !-
}

start:
    sei
    ROM_Only_Sequence()

    jsr spi.init
    jsr console.init

    jsr printBootScreen
#if WRITE_BOOT_CARTRIDGE
    jsr storeSoftware
#endif
    ldx #<msgRead
    ldy #>msgRead
    jsr console.println

    jsr cartridge.readBootSector

    ldx #<msgDone
    ldy #>msgDone
    jsr console.println

    jmp $0301

printBootScreen: {
    ldx #<msgWelcome
    ldy #>msgWelcome
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
    jmp console.println
}

ISR:
    rti

#if WRITE_BOOT_CARTRIDGE
storeSoftware: {
    ldx #<msgWrite
    ldy #>msgWrite
    jsr console.println

    jmp cartridge.write512BytesToRAM
}

msgWrite:
    .text "WRITE"
    .byte 0

#endif

msgWelcome:
    .text "JOFMODORE V0.01"
    .byte 0

msgRead:
    .text "LOAD"
    .byte 0
msgDone:
    .text "OK"
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

#import "jumptableptr.asm"
    * = $fffa "6502 vectors"
    .byte <ISR, >ISR
    .byte <start, >start
    .byte <IRQ_SR, >IRQ_SR