#include "spi.h"


void SPI_Master_Init(SPI_TypeDef *SPI){
    
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_BR,0x3);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_CPOL,1);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_CPHA,1);

    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_LSBFIRST,0);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_SSM,1);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_SSI,1);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_RXONLY,0);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_DFF,0);

    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_MSTR,1);

    WRITE_REG_FIELD(SPI->CR2,SPI_CR2_FRF,0);
    //WRITE_REG_FIELD(SPI->CR2,SPI_CR2_SSOE,1);
}
// Basado en master init, pero con la posibilidad de cambiar el CPOL, CPHA y BR
void SPI_Master_Config(SPI_TypeDef *SPI, uint8_t cpol, uint8_t cpha, uint8_t br){
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_SPE,0);       // BR, CPOL y CPHA solo se cambian 
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_BR,br);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_CPOL,cpol);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_CPHA,cpha);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_LSBFIRST,0);  // MSB primero
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_SSM,1);       // NSS por software
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_SSI,1);       // evita error MODF
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_RXONLY,0);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_DFF,0);       // 8 bits
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_BIDIMODE,0);  // full-duplex
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_CRCEN,0);     // sin CRC por hardware
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_MSTR,1);      // maestro
    WRITE_REG_FIELD(SPI->CR2,SPI_CR2_FRF,0);       // formato Motorola
    WRITE_REG_FIELD(SPI->CR2,SPI_CR2_SSOE,0);      // el SPI no maneja NSS
}


void SPI_Slave_Init(SPI_TypeDef *SPI){

    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_BR,3);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_CPOL,1);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_CPHA,1);

    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_LSBFIRST,0);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_SSM,1);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_SSI,0);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_RXONLY,0);
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_DFF,0);

    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_MSTR,0);

    WRITE_REG_FIELD(SPI->CR1,SPI_CR2_FRF,0);

}

void SPI_Slave_IRQ(SPI_TypeDef *SPI,uint8_t val){
    WRITE_REG_FIELD(SPI->CR2,SPI_CR2_RXNEIE,val);
}

void SPI_Enable(SPI_TypeDef *SPI,char enable){
    WRITE_REG_FIELD(SPI->CR1,SPI_CR1_SPE,enable);
}


char SPI_Master_Tranfer(SPI_TypeDef *SPI, uint8_t  *data, uint32_t size){
    uint32_t i=0;
    while(i<size){
        while (!(SPI->SR&SPI_SR_TXE));
        SPI->DR = data[i];
        while (!(SPI->SR&SPI_SR_RXNE));
        data[i] = (SPI->DR);
        i++;
    }
    while (!READ_REG_FIELD(SPI->SR,SPI_SR_TXE));
    while (READ_REG_FIELD(SPI->SR,SPI_SR_BSY));

    return(0);

}
 // Transmision de datos en modo maestro, sin recibir datos de vuelta.
char SPI_Master_Transmit(SPI_TypeDef *SPI, const uint8_t *data, uint32_t size){
    uint32_t i=0;
    while(i<size){
        while (!(SPI->SR&SPI_SR_TXE));
        SPI->DR = data[i];
        i++;
    }
    while (!READ_REG_FIELD(SPI->SR,SPI_SR_TXE));
    while (READ_REG_FIELD(SPI->SR,SPI_SR_BSY));

    (void)SPI->DR;   // leer DR y luego SR limpia RXNE y OVR
    (void)SPI->SR;

    return(0);
}
void SPI_Slave_Transmit(SPI_TypeDef* SPI,uint8_t txdata){
    SPI->DR = txdata;
}

uint8_t SPI_Slave_Receive(SPI_TypeDef* SPI){
    while (!SPI3->SR&SPI_SR_RXNE);
    return(SPI->DR);
}
