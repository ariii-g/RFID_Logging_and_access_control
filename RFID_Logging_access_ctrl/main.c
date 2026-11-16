#include <stdatomic.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inc/gpio_config.h"
#include "inc/spi.h"
#include "inc/system_config.h"
#include "inc/usart.h"
#include "inc/st7789.h"
#include "inc/tm_stm32f4_mfrc522.h"
#include "fatfs/diskio.h"
#include "inc/sd_port.h"
#include "fatfs/ff.h"   
#include "inc/rtc.h"
#include "inc/master.h"

/* Must be delcared volatile as the timer can update asynchounosly*/
extern void disk_timerproc(void);   // definida en fafts
volatile SW_Timers timers;
volatile uint8_t uart_rx_flag=0;
volatile uint8_t spi_rx_flag=0;
volatile uint8_t uart_rx_data=0;
uint8_t id[5];  
uint8_t last_id[4];
uint8_t has_last = 0;
uint8_t no_card_count = 0;
static FATFS fs;     // area de trabajo la tarjeta SD
static FIL   fil;    // un archivo abierto



/*The SysTick_Handler, was already defined as weak during the crt0.s init file, so when we define it here, 
the Vector table is updated whit the new address where the function is allocated, so that when and interrupt happen
the vector table knows where to find the SysTick_Handler*/
void SysTick_Handler(void){
  update_sw_timers(&timers);
  disk_timerproc(); // timer de la tarjeta SD, se actualiza cada 1ms
}


void USART2_IRQHandler(void){
  if(USART2->SR&USART_SR_RXNE){
    uart_rx_line_isr(USART2);// Cambio eco
  }
}
void hello_display(void){
  ST7789_DrawFilledRectangle(0,   0,   160, 20, RED);     // superior izquierda
  ST7789_DrawFilledRectangle(161, 0,   159, 20, GREEN);   // superior derecha
  ST7789_DrawFilledRectangle(0,   220, 160, 20, BLUE);    // inferior izquierda
  ST7789_DrawFilledRectangle(161, 220, 159, 20, YELLOW);  // inferior derecha
  ST7789_WriteString(24, 60,  "CONTROL DE ACCESO", Font_16x26, WHITE, BLACK);
  ST7789_WriteString(61, 110, "Acerque su tarjeta", Font_11x18, CYAN, BLACK);
}
void id_display(uint8_t * id){
  char id_str[20];
  sprintf(id_str, "ID: %02X %02X %02X %02X", id[0], id[1], id[2], id[3]);
  ST7789_Fill_Color( BLACK); 
  ST7789_DrawFilledRectangle(0,   0,   320, 20, BLUE);     // superior 
  ST7789_DrawFilledRectangle(0,   220, 320, 20, YELLOW);    // inferior
  ST7789_WriteString(95, 70,  "USUARIO", Font_16x26, WHITE, BLACK);
  ST7789_WriteString(75, 100, id_str, Font_11x18, CYAN, BLACK);
  ST7789_WriteString(40, 130, "ACCESO DENEGADO", Font_16x26, RED, BLACK);
}

int main()
{

  clock_config();
  SysTick_Init(1000);
  SysTick_enable_IrQ(1); 
  timers.sw_tmr1_period = 1000;
  timers.sw_tmr2_period = 100;
  /*Enable the CLK to the GPIOA and GPIOC, this needs to be done before the configuration opf the GPIO*/
  UART_GPIO_Config(USART2);
  UART_Init(USART2,115200);
  UART_RX_IRQ_EN(USART2,1);
  NVIC_EnableIRQ(USART2_IRQn);
  RTC_Init();
  ST7789_Port_Init();   // relojes, pines del puerto B y SPI2 
  ST7789_Init();        // reset por RST, configuracion y pantalla negra
  TM_MFRC522_Init();
  hello_display();

while(1)
 {
    char linea[41];
    if (uart_get_line(linea, sizeof(linea))) {
      uart_puts(USART2, "[");
      uart_puts(USART2, linea);
      uart_puts(USART2, "]\r\n");
    }
    if (timers.sw_tmr2_flag){
      timers.sw_tmr2_flag = 0;
      if (TM_MFRC522_Check(id) == MI_OK){
        no_card_count = 0;
        if(!has_last || memcmp(id, last_id, 4) != 0){
          memcpy(last_id, id, 4);
          has_last = 1;
          id_display(id);
        }
      }else{
        if (has_last){
          no_card_count++;
          if(no_card_count >= 20){ 
            no_card_count = 0;
            has_last = 0;
            ST7789_Fill_Color(BLACK);
            hello_display();
          }
        }
      }
    }
  }
}
