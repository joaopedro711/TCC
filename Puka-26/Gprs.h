// Gprs.h

#ifndef GPRS_H_
#define GPRS_H_

// GPRS USCI_A0(PC) integradas
extern volatile char gprs_fila[64]; //Espaço para a fila serial de entrada
extern volatile int gprs_pin, gprs_pout;       //Ponteiros para usar a fila

///////////////////////////////////////////////////////////////////
void gprs_dec16(int dt);
char gprs_cmdo(char *argc, char *argv, char limite);
char gprs_vazia(void);
void gprs_come_crlf(void);
char gprs_xereta(char *cha);
//////////////////////////////////////////////////////////////////////

void gprs_complete_str(char *msg);
void gprs_str(char *msg);
void gprs_char(char x);

void gprs_config(void);
char gprs_poe(char cha);
void gprs_cheia(void);
char gprs_tira(char *cha);
void ser0_config(char br);


#endif /* GPRS_H_ */
