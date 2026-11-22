/*
 *
 *   SysTick_Handler ............ base ejemplo 21-SPI-4
 *   USART2_IRQHandler .......... base del ejemplo, modificada para llamar a uart_rx_line_isr
 *   hello_display .............. escrito propio
 *   id_display ................. escrita propio, completada con IA
 *   init_SD ..............  propuestas con Claude.
 *   main.................. escrito propio, corrección de errores con IA
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
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
#include "inc/users.h"
#include "inc/eventos.h"
#include "inc/menu.h"

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
void id_display(uint8_t * id,const char * nombre, int acceso){
  char id_str[20];
  sprintf(id_str, "ID: %02X %02X %02X %02X", id[0], id[1], id[2], id[3]);
  ST7789_Fill_Color( BLACK); 
  ST7789_DrawFilledRectangle(0,   0,   320, 20, BLUE);     // superior 
  ST7789_DrawFilledRectangle(0,   220, 320, 20, YELLOW);    // inferior
  ST7789_WriteString(95, 70,  nombre, Font_11x18, WHITE, BLACK);
  ST7789_WriteString(75, 100, id_str, Font_11x18, CYAN, BLACK);
  if (acceso){
    ST7789_WriteString(30, 130, "ACCESO CONCEDIDO", Font_16x26, GREEN, BLACK);
  }else{
    ST7789_WriteString(40, 130, "ACCESO DENEGADO", Font_16x26, RED, BLACK);
  }
}
void init_SD(void){
  FRESULT fr = f_mount(&fs, "", 1);
  if (fr != FR_OK) {
    char msg[40];
    sprintf(msg, "Error al montar la SD: %d\r\n", fr);
    uart_puts(USART2, msg);
  }
  users_recover();   // repara un cambio a medias en USUARIOS.CSV
  master_init();     // asegura la maestra
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
  init_SD();                                // configura pines y SPI1
while(1)
 {   
    if (timers.sw_tmr2_flag){
      timers.sw_tmr2_flag = 0;
      if (TM_MFRC522_Check(id) == MI_OK){
        no_card_count = 0;
        if(!has_last || memcmp(id, last_id, 4) != 0){
          memcpy(last_id, id, 4);
          has_last = 1;
          char id_str[9];
          char nombre[41];
          id_to_str(id, id_str);                  
          int acceso = check_access(id_str, nombre, sizeof(nombre)); 
          id_display(id, nombre, acceso);

          if (is_master(id_str)) {
              menu_maestro();           
              ST7789_Fill_Color(BLACK);
          }
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
