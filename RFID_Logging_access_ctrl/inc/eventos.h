#ifndef EVENTOS_H
#define EVENTOS_H

#include <string.h>
#include "inc/master.h"
#include "inc/rtc.h"
#include "ff.h"
#define ARCHIVO_E "EVENTOS.CSV"
#define HEADERS_E "fecha,hora,ID,nombre,permiso,estado_acceso\n"
FRESULT append_event( const char *id, const char *nombre, int permiso, const char *estado_acceso);
#endif // EVENTOS_H