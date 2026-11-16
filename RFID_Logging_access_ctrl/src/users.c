#include "inc/users.h"

FRESULT find_user(const char *id, char *nombre,size_t nombre_len, int *permiso, int *encontrado){
  FRESULT res;
  FIL fil; 
  char line[128]; // buffer para leer cada línea del archivo
  *encontrado = 0; 
  res = f_open(&fil, ARCHIVO_U, FA_READ);
  if (res != FR_OK) {
    return res; // Error al abrir el archivo
   }
  f_gets(line, sizeof(line), &fil); // Leer la primera línea 
  while (f_gets(line, sizeof(line), &fil)) { // Leer cada línea del archivo
        size_t len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        // Separar información por comas
        char *coma1 = strchr(line, ',');
        if (coma1 == NULL) continue;        
        *coma1 = '\0';

        char *coma2 = strchr(coma1 + 1, ',');
        if (coma2 == NULL) continue;
        *coma2 = '\0';
        if (strcmp(line, id) == 0) {
            strncpy(nombre, coma1 + 1, nombre_len - 1);
            nombre[nombre_len - 1] = '\0';
            *permiso = atoi(coma2 + 1);
            *encontrado = 1;
            break;
        }
    }

    f_close(&fil);
    return FR_OK;
}
FRESULT add_user(const char *id, const char *nombre, int permiso)
{
    FRESULT res;
    FIL fil;
    char nombre_tmp[128];
    int permiso_tmp;
    int encontrado;

    if (strchr(nombre, ',') != NULL) {
        return FR_INVALID_NAME;//Nombre sin comas
    }

    // ID ya refistrado
    res = find_user(id, nombre_tmp, sizeof(nombre_tmp), &permiso_tmp, &encontrado);
    if (res != FR_OK && res != FR_NO_FILE) {
        return res;       
    }
    if (res == FR_OK && encontrado) {
        return FR_EXIST;          
    }

    res = f_open(&fil, ARCHIVO_U, FA_OPEN_APPEND | FA_WRITE);
    if (res != FR_OK) {
        return res;
    }
    // si el archivo está vacío, escribir encabezados
    if (f_size(&fil) == 0) {
        f_puts(HEADERS_U, &fil);
    }
    // escribir y cerrar el archivo
    f_printf(&fil, "%s,%s,%d\n", id, nombre, permiso);
    return f_close(&fil);
}
FRESULT delete_user(const char *id)
{
    FRESULT res;
    FIL in, out;
    char line[128];
    size_t id_len = strlen(id);
    int encontrado = 0;

    // Abrir orgiginal para lectura y temporal para escritura
    res = f_open(&in, ARCHIVO_U, FA_READ);
    if (res != FR_OK) {
        return res;
    }
    res = f_open(&out, ARCHIVO_TMP, FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        f_close(&in);           // cerrar el archivo original antes de salir
        return res;
    }

    // Copiar encabezados
    while (f_gets(line, sizeof(line), &in)) {
        if (strncmp(line, id, id_len) == 0 && line[id_len] == ',') {
            encontrado = 1;
            continue;           // omitir la línea que coincide con el ID
        }
        f_puts(line, &out);
    }

    // Copir menos id S
    f_close(&in);
    res = f_close(&out);
    if (res != FR_OK || !encontrado) {
        f_unlink(ARCHIVO_TMP);  // eliminar el archivo temporal si hubo un error o no se encontró el ID
        return (res != FR_OK) ? res : FR_NO_FILE;
    }

    // Reemplazar el archivo original con el temporal
    res = f_unlink(ARCHIVO_U);
    if (res != FR_OK) {
        return res;
    }
    return f_rename(ARCHIVO_TMP, ARCHIVO_U);
}
FRESULT update_user(const char *id, const char *nombre, int permiso)
{
    FRESULT res;
    FIL in, out;
    char line[128];
    size_t id_len = strlen(id);
    int encontrado = 0;

    // El nombre nuevo no puede tener comas
    if (strchr(nombre, ',') != NULL) {
        return FR_INVALID_NAME;
    }

    // Abrir original para lectura y temporal para escritura
    res = f_open(&in, ARCHIVO_U, FA_READ);
    if (res != FR_OK) {
        return res;
    }
    res = f_open(&out, ARCHIVO_TMP, FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        f_close(&in);
        return res;
    }

    // Copiar todas las líneas y la del ID se reemplaza por la nueva
    while (f_gets(line, sizeof(line), &in)) {
        if (strncmp(line, id, id_len) == 0 && line[id_len] == ',') {
            encontrado = 1;
            f_printf(&out, "%s,%s,%d\n", id, nombre, permiso);  // línea nueva
            continue;
        }
        f_puts(line, &out);
    }

    // Cerrar ambos archivos
    f_close(&in);
    res = f_close(&out);
    if (res != FR_OK || !encontrado) {
        f_unlink(ARCHIVO_TMP);
        return (res != FR_OK) ? res : FR_NO_FILE;
    }

    // Reemplazar el original con el temporal
    res = f_unlink(ARCHIVO_U);
    if (res != FR_OK) {
        return res;
    }
    return f_rename(ARCHIVO_TMP, ARCHIVO_U);
}
FRESULT users_recover(void)
{
    FILINFO fno;
    //  no hay nada que recuperar
    if (f_stat(ARCHIVO_TMP, &fno) != FR_OK) {
        return FR_OK;
    }
    //  el original es el válido
    if (f_stat(ARCHIVO_U, &fno) == FR_OK) {
        return f_unlink(ARCHIVO_TMP);
    }
    // el temporal es la lista buena
    return f_rename(ARCHIVO_TMP, ARCHIVO_U);
}
