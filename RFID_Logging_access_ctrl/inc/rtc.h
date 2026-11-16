#ifndef RTC_H
#define RTC_H
#include "system_config.h"
#include "inc/rtc.h"
#include "fatfs/ff.h"      // para el tipo DWORD
void RTC_Init(void);
void RTC_getdatetime(int *year, int *month, int *day, int *hour, int *minute, int *second);
DWORD get_fattime(void);  // para la funcion de fatfs
void RTC_getevents(char *fecha, char *hora);
#endif // RTC_H