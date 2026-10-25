#ifndef ST7789_PORT_H
#define ST7789_PORT_H
#include "spi.h"
#define ST7789_SPI   SPI2
#define ST7789_CS_PIN   9    // CS activo en bajo bit 9
#define ST7789_DC_PIN   1    // DC cambio entre comando y dato, alto bit 1
#define ST7789_RST_PIN  0    // PB0 - reset, activo en bajo  
// BSRR registro de 32 bits, los 16 bits menos significativos son para poner a 1 el pin correspondiente, y los 16 bits más significativos son para poner a 0 el pin correspondiente.
#define ST7789_Select()    (GPIOB->BSRR = (1U << (ST7789_CS_PIN + 16)))
#define ST7789_UnSelect()  (GPIOB->BSRR = (1U << ST7789_CS_PIN))
#define ST7789_DC_Clr()    (GPIOB->BSRR = (1U << (ST7789_DC_PIN + 16)))
#define ST7789_DC_Set()    (GPIOB->BSRR = (1U << ST7789_DC_PIN))
#define ST7789_RST_Clr()   (GPIOB->BSRR = (1U << (ST7789_RST_PIN + 16)))
#define ST7789_RST_Set()   (GPIOB->BSRR = (1U << ST7789_RST_PIN))

void ST7789_Port_Init(void);

#endif // ST7789_PORT_H