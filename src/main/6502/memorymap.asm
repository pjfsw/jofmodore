#importonce

.const SPI_DATA_PTR = $01

.const SPI_MOSI_L =         $0200
.const SPI_MOSI_H =         $0201
.const SPI_COUNT =          $0202
.const CONSOLE_POSITION =   $0203 // 2 bytes
.const CONSOLE_SCROLL =     $0205 // 2 bytes
.const CONSOLE_ID =         $0207 // 1 byte

.const VIA1_BASE = $D000
.const VIA1_DDRA = VIA1_BASE + 3
.const VIA1_DDRB = VIA1_BASE + 2
.const VIA1_PORTA = VIA1_BASE + 1
.const VIA1_PORTB = VIA1_BASE

.const VIA1_T1L = VIA1_BASE + 4
.const VIA1_T1H = VIA1_BASE + 5
.const VIA1_ACR = VIA1_BASE + 11

.const SPI_DDR = VIA1_DDRA
.const SPI_PORT = VIA1_PORTA
