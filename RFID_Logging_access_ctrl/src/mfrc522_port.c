#include "mfrc522_port.h"

void MFRC522_Port_Init(void){
    WRITE_REG_FIELD(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN, 1);
    WRITE_REG_FIELD(RCC->APB1ENR, RCC_APB1ENR_SPI3EN, 1);
    // habilitar el reloj del GPIOC Y BUS spi3
    volatile unsigned int dummy;
    dummy = (RCC->AHB1ENR);
    dummy = (RCC->APB1ENR);

    // Condiciones iniciales de los pines del puerto c para el control del MFRC522
    // CS en 1 ignora el bus SPI, RST en 1 indica que no se esta reseteando el MFRC522
    GPIOC->BSRR = (1U << MFRC522_CS_PIN) | (1U << MFRC522_RST_PIN); // Activacion al tiempo

    // Velocidad de los pines del puerto C para el control 
    WRITE_REG_FIELD(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR2, 0);
    WRITE_REG_FIELD(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR3, 0);
// Configuracion de los pines del puerto C para el control como salida
    WRITE_REG_FIELD(GPIOC->MODER, GPIO_MODER_MODER2, 1);// CS activo en bajo bit 2
    WRITE_REG_FIELD(GPIOC->MODER, GPIO_MODER_MODER3, 1);// RST activo en bajo bit 3
    // Configuracion de los pines spi del puerto C como funcion alternativa SPI3
    WRITE_REG_FIELD(GPIOC->AFR[1], GPIO_AFRH_AFSEL10,6);// SPI3_SCK
    WRITE_REG_FIELD(GPIOC->AFR[1], GPIO_AFRH_AFSEL11,6);// SPI3_MISO
    WRITE_REG_FIELD(GPIOC->AFR[1], GPIO_AFRH_AFSEL12, 6);// SPI3_MOSI
    // Configuracion de la velocidad de los pines SPI del puerto C MEDIA
    WRITE_REG_FIELD(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR10, 1);
    WRITE_REG_FIELD(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR11, 1);
    WRITE_REG_FIELD(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR12, 1);
    // Modo funcion alternativa 
    WRITE_REG_FIELD(GPIOC->MODER, GPIO_MODER_MODER10, 2);
    WRITE_REG_FIELD(GPIOC->MODER, GPIO_MODER_MODER11, 2);
    WRITE_REG_FIELD(GPIOC->MODER, GPIO_MODER_MODER12, 2);
    SPI_Master_Init(SPI3);
    SPI_Enable(SPI3, 1);
} 
void MFRC522_WriteReg(uint8_t reg, uint8_t value){
    uint8_t buf[2];
    buf[0] = (reg << 1) & 0x7E; // Dirección del registro a escribir
    buf[1] = value; // Valor a escribir en el registro
    MFRC522_Select();// Activar CS
    SPI_Master_Tranfer(SPI3, buf, 2); // Transmitir dirección y valor
    MFRC522_UnSelect();// Desactivar CS
}
uint8_t MFRC522_ReadReg(uint8_t reg){
    uint8_t buf[2];
    buf[0] = ((reg << 1) & 0x7E) | 0x80; // bit 7 en 1 indica lectura, bits 6-1 dirección del registro a leer, bit 0 en 0
    buf[1] = 0; // Valor de relleno para recibir el valor del registro
    MFRC522_Select();// Activar CS
    SPI_Master_Tranfer(SPI3, buf, 2); // Transmitir dirección y valor
    MFRC522_UnSelect();// Desactivar CS
    return buf[1]; // Devolver el valor leído del registro
}

