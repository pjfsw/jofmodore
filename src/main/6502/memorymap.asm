#importonce

.const SPI_DATA_PTR = $01

.const SPI_MOSI_L =         $0200
.const SPI_MOSI_H =         $0201
.const SPI_COUNT =          $0202
.const CONSOLE_POSITION =   $0203 // 2 bytes
.const CONSOLE_SCROLL =     $0205 // 2 bytes
.const CONSOLE_ID =         $0207 // 1 byte

.const VIA1_DDRA = $a003
.const VIA1_DDRB = $a002
.const VIA1_PORTA = $a001
.const VIA1_PORTB = $a000

.const VIA1_T1L = $a004
.const VIA1_T1H = $a005
.const VIA1_ACR = $a00b

.const SPI_DDR = VIA1_DDRA
.const SPI_PORT = VIA1_PORTA
