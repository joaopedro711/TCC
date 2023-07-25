// Puka - Estados.h


#ifndef SRCS_ESTADOS_H_
#define SRCS_ESTADOS_H_

volatile unsigned int estado_i;
volatile unsigned int estado_y;
volatile char estado_comando[8];

void estados_config();
void sel_estado();
void resete();
void dormente();
void vigilia();
void alerta_1();
void alerta_2();
void suspeito();
void apagar();
void ler_n();
void ler_n_m();
void code_erro();

#endif /* SRCS_ESTADOS_H_ */
