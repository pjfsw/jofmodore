#importonce
#import "memorymap.asm"
#import "gd_header.asm"
#import "65c02.asm"

.var img = LoadPicture("me2.png")

.label vb = $ff
.label SPRITECOUNT = 8
.label srcImage = 0

.macro gdWrite(addr) {
    lda #GD_SPI_WRITE | (addr/256)
    jsr BIOS_SPI_writeByte
    lda #(addr&255)
    jsr BIOS_SPI_writeByte
}

.macro gdWriteByte(addr, byte) {
    gd_select()
    {
        gdWrite(addr)
        lda #byte
        jsr BIOS_SPI_writeByte
    } gd_deselect()
}

.macro gdRead(addr) {
    lda #>addr
    jsr BIOS_SPI_writeByte
    lda #<addr
    jsr BIOS_SPI_writeByte
}

.function gdColorLo(r,g,b) {
    .return ((g & 7) << 5) | b;
}

.function gdColorHi(r,g,b) {
    .return (r << 2) | (g >> 3);
}

.function gdColor(isHi, r,g,b) {
    .if (isHi == 0) {
        .return gdColorLo(r,g,b)
    } else {
        .return gdColorHi(r,g,b)
    }
}

.pseudopc $0300 {
    .byte (prgEnd-$300)/256

    jsr setBackgroundColor
    jsr clearScreen
    jsr initSprite


!:
    { // Wait for next frame
    !:
        gd_select()
        {
            lda #GD_REGISTER
            jsr BIOS_SPI_writeByte
            lda #GD_VBLANK
            jsr BIOS_SPI_writeByte
            jsr BIOS_SPI_readByte
            sta vb
        } gd_deselect()
        lda vb
        bne !-
    !:
        gd_select()
        {
            lda #GD_REGISTER
            jsr BIOS_SPI_writeByte
            lda #GD_VBLANK
            jsr BIOS_SPI_writeByte
            jsr BIOS_SPI_readByte
            sta vb
        } gd_deselect()
        lda vb
        beq !-

        gd_select()
        {
            gdWrite(GD_RAM_SPR << 8)
           .for (var i = 0; i < SPRITECOUNT; i++) {
            // 4 x
                ldy i+x
                lda costable,y
                jsr BIOS_SPI_writeByte
                lda #%0000000
                jsr BIOS_SPI_writeByte
                ldy i+y
                lda sintable,y
                jsr BIOS_SPI_writeByte
                lda #srcImage << 1
                jsr BIOS_SPI_writeByte

                inc y+i
                inc x+i
            }
        } gd_deselect()

/*
        inc fadePos
        lda fadePos
        cmp #fadeBytes
        bne !+
        stz(fadePos)
    !:
        jsr setBackgroundColor*/
    }
    jmp !-

setBackgroundColor:
    gd_select()
    {
        gdWrite($280e)
        ldx fadePos
        lda fade,x
        jsr BIOS_SPI_writeByte
        ldx fadePos
        lda fade+fadeBytes,x
        jsr BIOS_SPI_writeByte
    } gd_deselect()
    rts

clearScreen:
    gd_select()
    {
        gdWrite(0)
        lda #' '
        sta SPI_DATA_PTR
        stz(SPI_COUNT)
        ldx #10
    !:
        phx()
        jsr BIOS_SPI_writeByteManyTimes
        plx()
        dex
        bne !-
    } gd_deselect()
    gd_select()
    {
        gdWrite($2806)
        lda #0
        jsr BIOS_SPI_writeByte
        lda #0
        jsr BIOS_SPI_writeByte
    } gd_deselect()

    rts

initSprite:
    gd_select()
    {
        gdWrite(GD_RAM_SPRIMG << 8)
        lda #<spriteData
        sta SPI_DATA_PTR
        lda #>spriteData
        sta SPI_DATA_PTR+1
        stz(SPI_COUNT)
        jsr BIOS_SPI_writeBytes
    } gd_deselect()

    gd_select()
    {
        gdWrite(GD_RAM_SPRPAL << 8)
        lda #<spriteColor
        sta SPI_DATA_PTR
        lda #>spriteColor
        sta SPI_DATA_PTR+1
        stz(SPI_COUNT)
        jsr BIOS_SPI_writeBytes
        jsr BIOS_SPI_writeBytes
    } gd_deselect()

    gdWriteByte((GD_REGISTER << 8) | GD_SPR_DISABLE, 0)

   gd_select()
    {
        gdWrite(GD_RAM_SPR << 8)
        ldx #0
        {
        !:
            phx()
            // X bit 0-7
            lda #200
            jsr BIOS_SPI_writeByte
            // X high + palette derp
            lda #%11100001
            jsr BIOS_SPI_writeByte
            // Y bit 0-7
            lda #0
            jsr BIOS_SPI_writeByte
            // Y bit 8 +  sourcei mage
            lda #%00001100
            jsr BIOS_SPI_writeByte
            plx()
            dex
            bne !-
        }
    } gd_deselect()
    rts

    .align  $100

sintable:
    .fill 256, 100*sin(i*PI/128)+138

costable:
    .fill 256, 100*cos(i*PI/128)+138

spriteData:
    .for (var y = 0; y < 16; y++) {
        .for (var x = 0; x < 16; x++) {
            .var pixel = img.getPixel(x,y)
            .var r = pixel >> 16
            .var g = (pixel >> 8) & 255
            .var b = pixel & 255
            .byte b
        }
    }

spriteColor:
    .for (var i = 0; i < 256; i++) {
        .byte gdColorLo(i/8,i/8,i/8)
        .byte gdColorHi(i/8,i/8,i/8)
    }


fade:
    .for (var i = 0 ; i < 2; i++) {
        .for (var j = 0; j < 32; j++) {
            .byte gdColor(i, j/2,0,j/2)
        }
        .for (var j = 0; j < 32; j++) {
            .byte gdColor(i, (31-j)/2,0,(31-j)/2)
        }
    }
.label fadeBytes = (*-fade)/2

fadePos:
    .byte 0
x:
    .for (var i = 0; i < SPRITECOUNT; i++) {
        .byte i*255/SPRITECOUNT
    }
y:
    .for (var i = 0; i < SPRITECOUNT; i++) {
        .byte i*255/SPRITECOUNT
    }
prgEnd:
}
