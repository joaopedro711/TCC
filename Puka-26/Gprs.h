// Gprs.h

#ifndef GPRS_H_
#define GPRS_H_

// GPRS USCI_A0(PC) integradas
extern volatile char gprs_fila[GPRS_FILA_TAM]; //Espa�o para a fila serial de entrada
extern volatile int gprs_pin, gprs_pout;       //Ponteiros para usar a fila

void gprs_str(char *msg);
void gprs_char(char x);

void gprs_config(void);
char gprs_poe(char cha);
void gprs_cheia(void);
char gprs_tira(char *cha);
void ser0_config(char br);


#endif /* GPRS_H_ */
