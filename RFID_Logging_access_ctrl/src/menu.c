#include "inc/menu.h"

static void menu_opciones(void){
    uart_puts(USART2, "\r\n--  Bienvenido Administrador de Accesso ;)-- \r\n"); 
    uart_puts(USART2, "\r\n--  Menu Principal, registra el número de opción--\r\n");
    uart_puts(USART2, "1.      Registrar usuario\r\n");
    uart_puts(USART2, "2.      Modificar usuario\r\n");
    uart_puts(USART2, "3.      Eliminar usuario\r\n");
    uart_puts(USART2, "4.      Salir\r\n");
    uart_puts(USART2, "Opcion: ");
}
static int leer_tarjeta(char *id_str){
    uint8_t id[5];
    char linea[41];

    uart_puts(USART2, "Acerque la tarjeta del usuario, escribe c para cancelar\r\n");

    while (1) {
        if (uart_get_line(linea, sizeof(linea)) && strcmp(linea, "c") == 0) {
            uart_puts(USART2, "Cancelado\r\n");
            return 0;
        }
        if (TM_MFRC522_Check(id) == MI_OK) {
            id_to_str(id, id_str);
            if (!is_master(id_str)) {
                uart_puts(USART2, "Tarjeta leida: ");
                uart_puts(USART2, id_str);
                uart_puts(USART2, "\r\n");
                return 1;
            }
            // es la maestra: la ignoramos y seguimos esperando
        }
        SysTick_Delay_us(100); 
    }
}

static int leer_permiso(void){
    char linea[41];
    while (1) {
        uart_puts(USART2, "Permiso del usuario, escribe 1 para conceder acceso, 0 para denegar \r\n");
        while (!uart_get_line(linea, sizeof(linea)));

        if (strcmp(linea, "1") == 0) {
            return 1;
        }
        if (strcmp(linea, "0") == 0) {
            return 0;
        }
        uart_puts(USART2, "Valor no valido\r\n");
    }
}
static void menu_eliminar(void){
    char id_str[9];
    char nombre[41];
    char resp[41];
    int permiso, encontrado;
    FRESULT res;

    // leer tarjeta
    if (!leer_tarjeta(id_str)) {
        return;
    }
    // no se elimina la tarjeta maestra
    if (!user_modify(id_str)) {
        uart_puts(USART2, "Error, la tarjeta maestra no se puede eliminar\r\n");
        return;
    }

    // buscar usuario
    res = find_user(id_str, nombre, sizeof(nombre), &permiso, &encontrado);
    if (res != FR_OK || !encontrado) {
        uart_puts(USART2, "Esta tarjeta no esta registrada\r\n");
        return;
    }
    //eliminar usuario
    uart_puts(USART2, "Eliminar a ");
    uart_puts(USART2, nombre);
    uart_puts(USART2, " confirmar (SI/NO): ");
    while (!uart_get_line(resp, sizeof(resp)));
    if (strcmp(resp, "SI") != 0) {
        uart_puts(USART2, "Cancelado\r\n");
        return;
    }
    res = delete_user(id_str);
    if (res == FR_OK) {
        uart_puts(USART2, "Usuario eliminado\r\n");
    } else if (res == FR_NO_FILE) {
        uart_puts(USART2, "Error, el usuario no se encontro\r\n");
    } else {
        uart_puts(USART2, "Error al escribir en la SD\r\n");
    }
}
static void menu_modificar(void){
    char id_str[9];
    char nombre[41];
    char nuevo[41];
    int permiso, encontrado;
    FRESULT res;

    if (!leer_tarjeta(id_str)) {
        return;
    }
    if (!user_modify(id_str)) {
        uart_puts(USART2, "Error, la tarjeta maestra no se puede modificar\r\n");
        return;
    }

    // Buscar y mostrar los datos actuales
    res = find_user(id_str, nombre, sizeof(nombre), &permiso, &encontrado);
    if (res != FR_OK || !encontrado) {
        uart_puts(USART2, "Esta tarjeta no esta registrada\r\n");
        return;
    }
    uart_puts(USART2, "Usuario actual: ");
    uart_puts(USART2, nombre);
    uart_puts(USART2, permiso ? " con acceso\r\n" : " sin acceso\r\n");

    //  Nombre nuevo conserva el actual
    uart_puts(USART2, "Nuevo nombre y apellido, escribe - para conservar: ");
    while (!uart_get_line(nuevo, sizeof(nuevo)));
    if (strcmp(nuevo, "-") == 0) {
        strcpy(nuevo, nombre);
    }
    //  Permiso nuevo 
    permiso = leer_permiso();
    // Guardar
    res = update_user(id_str, nuevo, permiso);
    if (res == FR_OK) {
        uart_puts(USART2, "Usuario modificado\r\n");
    } else if (res == FR_INVALID_NAME) {
        uart_puts(USART2, "Error, el nombre no puede tener comas\r\n");
    } else if (res == FR_INVALID_PARAMETER) {
        uart_puts(USART2, "Error, escribe nombre y apellido\r\n");
    } else if (res == FR_DENIED) {
        uart_puts(USART2, "Error, ese nombre ya pertenece a otra tarjeta\r\n");
    } else {
        uart_puts(USART2, "Error al escribir en la SD\r\n");
    }
}
static void menu_registrar(void){
    char id_str[9];
    char nombre[41];
    char nombre_tmp[41];
    int permiso, permiso_tmp, encontrado;
    FRESULT res;
    if (!leer_tarjeta(id_str)) {
        return;
    }

    //Verificar si ya esta registrada
    res = find_user(id_str, nombre_tmp, sizeof(nombre_tmp), &permiso_tmp, &encontrado);
    if (res == FR_OK && encontrado) {
        uart_puts(USART2, "Esta tarjeta ya pertenece a: ");
        uart_puts(USART2, nombre_tmp);
        uart_puts(USART2, "\r\n");
        return;
    }

    // Nombre del usuario
    uart_puts(USART2, "Nombre y apellido: ");
    while (!uart_get_line(nombre, sizeof(nombre)));
    // Permiso del usuario
    permiso =leer_permiso();
    // Añadir usuario a la SD
    res = add_user(id_str, nombre, permiso);
    if (res == FR_OK) {
        uart_puts(USART2, "Usuario registrado\r\n");
    } else if (res == FR_INVALID_NAME) {
        uart_puts(USART2, "Error, el nombre no puede tener comas\r\n");
    } else if (res == FR_EXIST) {
        uart_puts(USART2, "Error, la tarjeta ya esta registrada\r\n");
    } else if (res == FR_INVALID_PARAMETER) {
        uart_puts(USART2, "Error, escribe nombre y apellido\r\n");
    } else if (res == FR_DENIED) {
        uart_puts(USART2, "Error, esa persona ya tiene una tarjeta\r\n");
    }else {
        uart_puts(USART2, "Error al escribir en la SD\r\n");
    }
    
}

void menu_maestro(void){
    char opcion[41];

    ST7789_Fill_Color(BLACK);// mostrar modo maestro
    ST7789_DrawFilledRectangle(0,   0,   320, 20, CYAN);     // superior 
    ST7789_DrawFilledRectangle(0,   220, 320, 20, CYAN);    // inferior
    ST7789_WriteString(60, 100, "MODO MAESTRO", Font_16x26, WHITE, BLACK);
    ST7789_WriteString(70, 140, "Control en el PC", Font_11x18, YELLOW, BLACK);
    uart_get_line(opcion, sizeof(opcion));
    while (1) {
        menu_opciones();// MOSTRAR OPCIONES HATA SALIR
        while (!uart_get_line(opcion, sizeof(opcion)));   // esperar la opción
        if (strcmp(opcion, "1") == 0) {
            uart_puts(USART2, "Registrar\r\n");
            menu_registrar();
            
        } else if (strcmp(opcion, "2") == 0) {
            uart_puts(USART2, "Modificar\r\n");
            menu_modificar();
        } else if (strcmp(opcion, "3") == 0) {
            uart_puts(USART2, "Eliminar\r\n");
            menu_eliminar();
        } else if (strcmp(opcion, "4") == 0) {
            uart_puts(USART2, "Saliendo del modo maestro\r\n");
            return;
        } else {
            uart_puts(USART2, "Opcion no valida\r\n");
        }
    }
}
