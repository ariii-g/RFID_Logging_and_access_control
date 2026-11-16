#ifndef _USART_H_
#define _USART_H_

#include "system_config.h"
#include <stddef.h>
#include <string.h>
static volatile uint8_t uart_line_len   = 0;
static volatile uint8_t uart_line_ready = 0;
static volatile char    uart_line[41];

void UART_GPIO_Config(USART_TypeDef* USART);

void UART_Init(USART_TypeDef* USART, uint32_t baudrate);
void UART_RX_IRQ_EN(USART_TypeDef* USART,uint8_t enable);


void uart_putc(USART_TypeDef* USART,char dat);
char uart_tstc(USART_TypeDef* USART);
char uart_getc(USART_TypeDef* USART);

void uart_puts(USART_TypeDef* USART,const char* str);
void uart_rx_line_isr(USART_TypeDef* USART);
int  uart_get_line(char *dst, size_t n);


#endif 