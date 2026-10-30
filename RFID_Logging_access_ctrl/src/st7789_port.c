#include "st7789_port.h"

void ST7789_Port_Init(void){
    volatile uint32_t dummy;

    WRITE_REG_FIELD(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN, 1);// Activacion reloj del puerto 
    WRITE_REG_FIELD(RCC->APB1ENR, RCC_APB1ENR_SPI2EN, 1);// Activacion Bus SPI2
    dummy = RCC->AHB1ENR;   
    dummy = RCC->APB1ENR;
    // Condiciones iniciales de los pines del puerto B para el control del display ST7789
    // CS en 1 ignora el bus SPI, DC en 1 indica que se enviaran datos, RST en 1 indica que no se esta reseteando el display
    GPIOB->BSRR = (1U << ST7789_CS_PIN) | (1U << ST7789_DC_PIN) | (1U << ST7789_RST_PIN);
    // Velocidad de los pines del puerto B para el control 
    WRITE_REG_FIELD(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR9, 0);
    WRITE_REG_FIELD(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR1, 0);
    WRITE_REG_FIELD(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR0, 0);
// Configuracion de los pines del puerto B para el control como salida
    WRITE_REG_FIELD(GPIOB->MODER, GPIO_MODER_MODER9, 1);
    WRITE_REG_FIELD(GPIOB->MODER, GPIO_MODER_MODER1, 1);
    WRITE_REG_FIELD(GPIOB->MODER, GPIO_MODER_MODER0, 1);
    // Configuracion de los pines spi del puerto B como funcion alternativa SPI2
    WRITE_REG_FIELD(GPIOB->AFR[1], GPIO_AFRH_AFSEL13, 5);// SPI2_SCK
    WRITE_REG_FIELD(GPIOB->AFR[1], GPIO_AFRH_AFSEL15, 5);// SPI2_MOSI

    // Configuracion de la velocidad de los pines SPI del puerto B MEDIA
    WRITE_REG_FIELD(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR13, 1);
    WRITE_REG_FIELD(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR15, 1);

    // Modo funcion alternativa 
    WRITE_REG_FIELD(GPIOB->MODER, GPIO_MODER_MODER13, 2);
    WRITE_REG_FIELD(GPIOB->MODER, GPIO_MODER_MODER15, 2);
    // Inicializacion del bus SPI2 en modo maestro, CPOL=1, CPHA=1, 8 bits d
    // datos, MSB primero, software slave management habilitado, SSI=1
    SPI_Master_Init(SPI2);   
    SPI_Enable(SPI2, 1); 
}
// Enviar comando
void ST7789_Port_WriteCmd(uint8_t cmd){
    ST7789_Select();                          // CS = 0 la pantalla escucha
    ST7789_DC_Clr();                          // DC = 0 el byte es un comando
    SPI_Master_Transmit(SPI2, &cmd, 1); // envia y espera BSY = 0 
    ST7789_UnSelect();                        // CS = 1 fin de la transaccion
}

// Enviar datos o parametros de un comando
void ST7789_Port_WriteData(const uint8_t *data, uint32_t size){
    ST7789_Select();                          // CS = 0
    ST7789_DC_Set();                          // DC = 1: los bytes son datos
    SPI_Master_Transmit(SPI2, data, size);
    ST7789_UnSelect();                        // CS = 1
}