#ifndef _SPI_H_
#define _SPI_H_

#include "system_config.h"

#define SPI_TRANSFER_REG

void SPI_Slave_IRQ(SPI_TypeDef *SPI,uint8_t val);

void SPI_Master_Init(SPI_TypeDef * SPI);
// Divisores del reloj del bus para el campo BR de SPI_CR1
#define SPI_BR_DIV2    0
#define SPI_BR_DIV4    1
#define SPI_BR_DIV8    2
#define SPI_BR_DIV16   3
#define SPI_BR_DIV32   4
#define SPI_BR_DIV64   5
#define SPI_BR_DIV128  6
#define SPI_BR_DIV256  7
void SPI_Master_Config(SPI_TypeDef *SPI, uint8_t cpol, uint8_t cpha, uint8_t br);// SD
void SPI_Slave_Init(SPI_TypeDef *SPI);

void SPI_Enable(SPI_TypeDef *SPI,char enable);

char SPI_Master_Tranfer(SPI_TypeDef *SPI, uint8_t  *data, uint32_t size);
char SPI_Master_Transmit(SPI_TypeDef *SPI, const uint8_t *data, uint32_t size);
void SPI_Slave_Transmit(SPI_TypeDef* SPI,uint8_t txdata);

uint8_t SPI_Slave_Receive(SPI_TypeDef* SPI);







#endif