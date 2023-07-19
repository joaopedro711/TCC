// Puka - Rtc.h

#ifndef RTC_H_
#define RTC_H_

/////////////// RTC DS 3231 - Constantes
#define RTC_ADR  0x68  //Endere�o RTC DS3231
#define RTC_EWR  0xD0  //RTC para escrita (0x68<<1)
#define RTC_ERD  0xD1  //RTC para leitura (0x68<<1 + 1)

char vetor_rtc[9];

void rtc_wr_vet(char reg, char *vt, char qtd);
void rtc_rd_vet(char reg, char *vt, char qtd);
char rtc_rd(char reg);
void rtc_wr(char reg, char dado);
char rtc_come_vig(char ini, char *vt, char qtd);




#endif /* RTC_H_ */
