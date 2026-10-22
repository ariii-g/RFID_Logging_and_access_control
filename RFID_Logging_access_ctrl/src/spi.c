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

void SPI_Slave_Transmit(SPI_TypeDef* SPI,uint8_t txdata){
    SPI->DR = txdata;
}

uint8_t SPI_Slave_Receive(SPI_TypeDef* SPI){
    while (!SPI3->SR&SPI_SR_RXNE);
    return(SPI->DR);
}
