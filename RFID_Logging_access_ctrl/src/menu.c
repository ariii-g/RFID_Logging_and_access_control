#include <string.h>

#include "inc/menu.h"
#include "inc/usart.h"
#include "inc/st7789.h"

static void menu_opciones(void){
    uart_puts(USART2, "\r\n--Bienvenido Administrador de Accesso ;)-- \r\n"); 
    uart_puts(USART2, "\r\n--Menu Principal, registra el número de opción--\r\n");
    uart_puts(USART2, "1. Registrar usuario\r\n");
    uart_puts(USART2, "2. Modificar usuario\r\n");
    uart_puts(USART2, "3. Eliminar usuario\r\n");
    uart_puts(USART2, "4. Salir\r\n");
    uart_puts(USART2, "Opción: ");
}

void menu_maestro(void){
    char opcion[41];

    ST7789_Fill_Color(BLACK);// mostrar modo maestro
    ST7789_DrawFilledRectangle(0,   0,   320, 20, CYAN);     // superior 
    ST7789_DrawFilledRectangle(0,   220, 320, 20, CYAN);    // inferior
    ST7789_WriteString(40, 100, "MODO MAESTRO", Font_16x26, WHITE, BLACK);
    ST7789_WriteString(30, 140, "Control en el PC", Font_11x18, YELLOW, BLACK);
    uart_get_line(opcion, sizeof(opcion));
    while (1) {
        menu_opciones();// MOSTRAR OPCIONES HATA SALIR
        while (!uart_get_line(opcion, sizeof(opcion)));   // esperar la opción
        if (strcmp(opcion, "1") == 0) {
            uart_puts(USART2, "Registrar\r\n");
        } else if (strcmp(opcion, "2") == 0) {
            uart_puts(USART2, "Modificar\r\n");
        } else if (strcmp(opcion, "3") == 0) {
            uart_puts(USART2, "Eliminar\r\n");
        } else if (strcmp(opcion, "4") == 0) {
            uart_puts(USART2, "Saliendo del modo maestro\r\n");
            return;
        } else {
            uart_puts(USART2, "Opcion no valida\r\n");
        }
    }
}