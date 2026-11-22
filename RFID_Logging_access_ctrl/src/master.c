/*
 *
 *   Todas las funciones propuestas con Claude, adaptadas y probadas previas a compilación.
 */
#include <stdio.h>
#include "inc/users.h"
#include "inc/master.h"
#include "inc/eventos.h"
int is_master(const char * id){
  return strcmp(id, master_id) == 0;
}
int user_modify(const char *id){
 return !is_master(id);
}
int check_access(const char *id, char *nombre, size_t nlen){
    int permiso = 0;
    int encontrado = 0;

    if (is_master(id)) {
        strncpy(nombre, master_name, nlen - 1);
        nombre[nlen - 1] = '\0';
        append_event(id, nombre, master_permission, "CONCEDIDO");
        return 1;
    }
    if (find_user(id, nombre, nlen, &permiso, &encontrado) != FR_OK) {
        strncpy(nombre, "ERROR SD", nlen - 1);
        nombre[nlen - 1] = '\0';
        append_event(id, nombre, 0, "DENEGADO");
        return 0;
    }
    if (!encontrado) {
        strncpy(nombre, "DESCONOCIDO", nlen - 1);
        nombre[nlen - 1] = '\0';
        append_event(id, nombre, 0, "DENEGADO");
        return 0;
    }
    if (permiso == 1) {
        append_event(id, nombre, permiso, "CONCEDIDO");
        return 1;
    }
    append_event(id, nombre, permiso,"DENEGADO");
    return 0;
}FRESULT master_init(void){
{
    FRESULT res;
    res = add_user(master_id, master_name, master_permission);
    if (res == FR_EXIST) {
        return FR_OK;      //Ya registrado
    }
    return res;            //agregar usuario maestro
}
}
void id_to_str(const uint8_t *id, char *str){
  sprintf(str, "%02X%02X%02X%02X", id[0], id[1], id[2], id[3]);
}