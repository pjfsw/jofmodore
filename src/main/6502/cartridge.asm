#importonce

#import "spi.asm"

    .label CART_SELECT_ID = 3
    .label CART_LOAD_TO_ADDRESS = $0300

    .label CART_READ = $03
    .label CART_WRITE = $02
    .label CART_RDMR = $05
    .label CART_WRMR = $01

    .label CART_SEQUENTIAL_MODE = %01000000

.macro cart_select() {
    ldx #CART_SELECT_ID
    jsr spi.selectDevice
}

.macro cart_deselect() {
    ldx #0
    jsr spi.selectDevice
}

.namespace cartridge {
readBootSector:
    jsr setSequentialMode

    cart_select()
    {
        // Read command
        lda #CART_READ
        jsr spi.writeByte

        // Set the default load adress on cartridge
        jsr setCartBootAddress

        // Load data to beginning of RAM
        stz(SPI_DATA_PTR)
        lda #>CART_LOAD_TO_ADDRESS
        sta SPI_DATA_PTR+1

        // Ready 4 KB data into RAM
        ldx #4096/256
!:
        phx()
        // Read 256 bytes of data
        jsr spi.readMemoryPage
        inc SPI_DATA_PTR+1
        plx()
        dex
        bne !-
    } cart_deselect()

    rts

write512BytesToRAM: {
    jsr setSequentialMode

    cart_select()
    {
        // Write command
        lda #CART_WRITE
        jsr spi.writeByte

        jsr setCartBootAddress

        // Write 512 bytes of data
        ldx #2
    !:
        phx()
        lda #<testdata
        sta SPI_DATA_PTR
        lda #>testdata
        sta SPI_DATA_PTR+1
        lda #0
        sta SPI_COUNT
        jsr spi.writeBytes
        plx()
        inc testdata
        dex
        bne !-
        lda #0
        jsr spi.writeByte
    } cart_deselect()

    rts
}

// Set 24-bit adress = 0x000000
setCartBootAddress: {
    lda #0
    jsr spi.writeByte
    lda #0
    jsr spi.writeByte
    lda #0
    jsr spi.writeByte
    rts
}

setSequentialMode: {
    cart_select()
    {
        lda #CART_WRMR
        jsr spi.writeByte
        lda #CART_SEQUENTIAL_MODE
        jsr spi.writeByte
    } cart_deselect()
    rts
}

testdata:
    #import "loadedprg.asm"

testdataend:
}