#include "sd_port.h"

void sd_init(void){
    WRITE_REG_FIELD(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN, 1);
    WRITE_REG_FIELD(RCC->APB2ENR, RCC_APB2ENR_SPI1EN, 1);
    // habilitar el reloj del GPIOA  Y BUS spi1
    volatile unsigned int dummy;
    dummy = (RCC->AHB1ENR);
    dummy = (RCC->APB2ENR);
    SD_UnSelect(); // Activar CS
    WRITE_REG_FIELD(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEEDR8, 0);//Velocidad media cs
    WRITE_REG_FIELD(GPIOA->MODER, GPIO_MODER_MODER8, 1);// como salida
    //Configuracion de los pines spi del puerto A como funcion alternativa SPI1
    WRITE_REG_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL5 ,5);// SPI1_SCK
    WRITE_REG_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL6 ,5);// SPI1_MISO
    WRITE_REG_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL7 ,5);// SPI1_MOSI
    // Configuracion de la velocidad de los pines SPI del puerto A MEDIA
    WRITE_REG_FIELD(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEEDR5, 1);
    WRITE_REG_FIELD(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEEDR6, 1);
    WRITE_REG_FIELD(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEEDR7, 1);
    // pull-up en el MISO para que el bus SPI no quede flotando
    WRITE_REG_FIELD(GPIOA->PUPDR, GPIO_PUPDR_PUPD6, 1);
    // Modo funcion alternativa 
    WRITE_REG_FIELD(GPIOA->MODER, GPIO_MODER_MODER5, 2);
    WRITE_REG_FIELD(GPIOA->MODER, GPIO_MODER_MODER6, 2);
    WRITE_REG_FIELD(GPIOA->MODER, GPIO_MODER_MODER7, 2); 
    SPI_Master_Config(SD_SPI, 0, 0, SPI_BR_DIV256);
    SPI_Enable(SD_SPI, 1);
 
}