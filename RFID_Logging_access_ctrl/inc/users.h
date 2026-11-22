#ifndef USERS_H
#define USERS_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ff.h"
#define ARCHIVO_U "USUARIOS.CSV"
#define HEADERS_U "ID,nombre,permiso\n"
#define ARCHIVO_TMP "TEMP.CSV"
FRESULT find_user(const char *id, char *nombre,size_t nombre_len, int *permiso, int *encontrado);
FRESULT add_user(const char *id, const char *nombre, int permiso);
FRESULT delete_user(const char *id);
FRESULT update_user(const char *id, const char *nombre, int permiso);
FRESULT users_recover(void);
FRESULT find_name(const char *nombre, char *id, size_t id_len, int *permiso, int *encontrado);

#endif // USERS_H