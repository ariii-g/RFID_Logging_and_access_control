#ifndef MASTER_H
#define MASTER_H
#include "ff.h"
#include <string.h>
#include <stddef.h>
#define master_id "C3CD9C18"
#define master_name "ADMINISTRADOR"
#define master_permission 2

int is_master(const char * id);
int check_access(const char *id, char *nombre, size_t nlen);
int user_modify(const char *id);
FRESULT master_init(void);
#endif // MASTER_H