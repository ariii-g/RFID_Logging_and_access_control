/*
 *   append_event propuesta con Claude, adaptada y aprobada previa a compilación.
 */
#include "inc/eventos.h"


FRESULT append_event(const char *id, const char *nombre, int permiso, const char *estado_acceso){
  FRESULT res;
  FIL fil;
  char hora[9];
  char fecha[11];
  RTC_getevents(fecha, hora);
  // Abrir el archivo en modo append
  res = f_open(&fil, ARCHIVO_E, FA_OPEN_APPEND | FA_WRITE);
  if (res != FR_OK) {
    return res; // Error al abrir el archivo
  }
  if (f_size(&fil) == 0) {
    // Escribir encabezados si el archivo está vacío
    f_puts(HEADERS_E, &fil);
  }
  f_printf(&fil, "%s,%s,%s,%s,%d,%s\n", fecha, hora, id, nombre, permiso, estado_acceso);
  // Cerrar el archivo
  return f_close(&fil);
}

