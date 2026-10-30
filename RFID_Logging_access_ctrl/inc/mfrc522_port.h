#ifndef MFRC522_PORT_H
#define MFRC522_PORT_H
#include "spi.h"
#include "system_config.h"


#define MFRC522_CS_PIN   2    // CS activo en bajo bit 2
#define MFRC522_RST_PIN  3    // PC3 reset, activo en bajo

// BSRR registro de 32 bits, los 16 bits menos significativos son para poner a 1 el pin correspondiente, y los 16 bits más significativos son para poner a 0 el pin correspondiente.
#define MFRC522_Select()    (GPIOC->BSRR = (1U << (MFRC522_CS_PIN + 16)))
#define MFRC522_UnSelect()  (GPIOC->BSRR = (1U << MFRC522_CS_PIN))
#define MFRC522_RST_Clr()   (GPIOC->BSRR = (1U << (MFRC522_RST_PIN + 16)))
#define MFRC522_RST_Set()   (GPIOC->BSRR = (1U << MFRC522_RST_PIN)) 
void MFRC522_Port_Init(void);
void    MFRC522_WriteReg(uint8_t reg, uint8_t value);
uint8_t MFRC522_ReadReg(uint8_t reg);
void MFRC522_IRQ_Init(void);
#endif // MFRC522_PORT_H