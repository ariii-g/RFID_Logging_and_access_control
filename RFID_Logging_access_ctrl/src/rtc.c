#include "inc/rtc.h"

void RTC_Init(void){
    // Configuración general del oscilador
    WRITE_REG_FIELD(RCC->APB1ENR,RCC_APB1ENR_PWREN,1);
    volatile unsigned int dummy;
    dummy = (RCC->APB1ENR);
    WRITE_REG_FIELD(PWR->CR,PWR_CR_DBP,1);

    WRITE_REG_FIELD(RCC->BDCR,RCC_BDCR_LSEON,1);
    while (!(READ_REG_FIELD(RCC->BDCR,RCC_BDCR_LSERDY)));
    WRITE_REG_FIELD(RCC->BDCR,RCC_BDCR_RTCSEL,1); // LSE
    WRITE_REG_FIELD(RCC->BDCR,RCC_BDCR_RTCEN,1);
    // Desbloquear registro fecha y hora
    WRITE_REG(RTC->WPR, 0xCA);
    WRITE_REG(RTC->WPR,0x53);
    WRITE_REG_FIELD(RTC->ISR, RTC_ISR_INIT,1);
    while(!(READ_REG_FIELD(RTC->ISR,RTC_ISR_INITF)));

    WRITE_REG_FIELD(RTC-> PRER, RTC_PRER_PREDIV_S, 255);//Sincrono
    WRITE_REG_FIELD(RTC-> PRER, RTC_PRER_PREDIV_A, 127);//Asincrono

    WRITE_REG_FIELD(RTC->CR, RTC_CR_FMT,0);//24 horas
    WRITE_REG(RTC->TR, 0x00070000);
    WRITE_REG(RTC->DR,0X00267118);
    WRITE_REG_FIELD(RTC->ISR, RTC_ISR_INIT,0);//SALIR
    WRITE_REG(RTC->WPR,0xFF);
}
void RTC_getdatetime(int *year, int *month, int *day, int *hour, int *minute, int *second){
    while (!(READ_REG_FIELD(RTC->ISR, RTC_ISR_RSF)));
    uint32_t tr = RTC->TR;
    uint32_t dr = RTC->DR;

    *hour = ((tr >> 20) & 0x3) * 10 + ((tr >> 16) & 0xF);
    *minute = ((tr >> 12) & 0x7) * 10 + ((tr >> 8) & 0xF);
    *second = ((tr >> 4) & 0x7) * 10 + (tr & 0xF);

    *day = ((dr >> 4) & 0x3) * 10 + (dr & 0xF);
    *month = ((dr >> 12) & 0x1) * 10 + ((dr >> 8) & 0xF);
    *year =2000 + (((dr >> 20) & 0xF) * 10 + ((dr >> 16) & 0xF));
}
DWORD get_fattime(void){
    int year, month, day, hour, minute, second;
    RTC_getdatetime(&year, &month, &day, &hour, &minute, &second);
    return ((DWORD)(year - 1980) << 25) | ((DWORD)month << 21) | ((DWORD)day << 16) |
           ((DWORD)hour << 11) | ((DWORD)minute << 5) | ((DWORD)(second / 2));
}