#ifndef RTC_H
#define RTC_H
#include "system_config.h"
void RTC_Init(void);
void RTC_getdatetime(int *year, int *month, int *day, int *hour, int *minute, int *second);
#endif // RTC_H