#ifndef MASTER_H
#define MASTER_H
#include "ff.h"
#include <string.h>
#define master_id "C3CD9C18"
#define master_name "Admin"
#define master_permission 2

int is_master(const char * id);
int check_access(const char * id);
int user_modify(const char *id);
FRESULT master_init(void);
#endif // MASTER_H