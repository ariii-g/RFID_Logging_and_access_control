#ifndef SD_PORT_H
#define SD_PORT_H
#include "spi.h"
#include "system_config.h"

#define SD_CS_PIN   8    // CS activo en bajo bit 8
#define SD_SPI   SPI1    // periferico SPI que usa la SD
// BSRR registro de 32 bits, los 16 bits menos significativos son para poner a 1 el pin correspondiente, y los 16 bits más significativos son para poner a 0 el pin correspondiente.
#define SD_Select()    (GPIOA->BSRR = (1U << (SD_CS_PIN + 16)))
#define SD_UnSelect()  (GPIOA->BSRR = (1U << SD_CS_PIN))


void sd_init(void);
#endif // SD_PORT_H