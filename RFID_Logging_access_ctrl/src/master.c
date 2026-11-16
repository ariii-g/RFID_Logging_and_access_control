#include "inc/master.h"
int is_master(const char * id){
  return strcmp(id, master_id) == 0;
}
int user_modify(const char *id){
 return !is_master(id);
}
int check_access(const char * id){
    char nombre[128];
    int permiso;
    int encontrado;
  if (is_master(id)) {
    append_event(id, master_name, master_permission,"CONCEDIDO" );
    return 1; // Acceso concedido para el usuario maestro
  } 
  if (find_user(id, nombre, sizeof(nombre), &permiso, &encontrado) != FR_OK ) {
      append_event(id, "ERROR", 0, "DENEGADO");
      return 0; // Error al buscar el usuario
    }
  if (encontrado && permiso == 1) {
    append_event(id, nombre, permiso, "CONCEDIDO");
    return 1; // Acceso concedido
  }
  append_event(id,"DESCONOCIDO", 0, "DENEGADO");
  return 0;
}
FRESULT master_init(void){
{
    FRESULT res;
    res = add_user(master_id, master_name, master_permission);
    if (res == FR_EXIST) {
        return FR_OK;      //Ya registrado
    }
    return res;            //agregar usuario maestro
}
}
