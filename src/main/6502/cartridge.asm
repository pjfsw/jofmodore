#importonce

#import "spi.asm"

    .label CART_LOAD_TO_ADDRESS = $0300

    .label CART_READ = $03
    .label CART_WRITE = $02
    .label CART_RDMR = $05
    .label CART_WRMR = $01

    .label CART_SEQUENTIAL_MODE = %01000000

.macro cart_select() {
    ldx #3
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

        // Read 256 bytes of data
        jsr spi.readMemoryPage
        inc SPI_DATA_PTR+1
        jsr spi.readMemoryPage
    } cart_deselect()

    rts

write256BytesToRAM: {
    jsr setSequentialMode

    cart_select()
    {
        // Write command
        lda #CART_WRITE
        jsr spi.writeByte

        jsr setCartBootAddress

        // Write soem data
        ldx #2
    !:
        phx()
        lda #<testdata
        sta SPI_DATA_PTR
        lda #>testdata
        sta SPI_DATA_PTR+1
        lda #testdataend-testdata
        sta SPI_COUNT
        jsr spi.writeBytes
        plx()
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
    .text "THIS IS STORED IN RAM AND OCCUPIES A LOT OF SPACE"
testdataend:
}