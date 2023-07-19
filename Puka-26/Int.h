// Puka - Int.c
// Todas as rotinas de interrup��o

#ifndef INT_H_
#define INT_H_

// Timers - Vari�veis Globais
extern volatile unsigned int  crono_cont;   //Contador para o Cron�metro (TA1)
extern volatile unsigned char flag_c_seg;   //Flag Cent�simos de segundos
extern volatile unsigned char flag_d_seg;   //Flag Cent�simos de segundos
extern volatile unsigned char flag_seg;     //Flag Segundos
extern volatile unsigned char cont_c_seg;   //Contar cent�simos de segundo;
extern volatile unsigned char cont_d_seg;   //Contar cent�simos de segundo;
extern volatile unsigned char cont_seg;     //Contar segundos;
extern volatile unsigned char cont_min;     //Contar minutos;
extern volatile char lcd_flag;              //Indica que precisa atualizar LCD
extern volatile char i2c_tranca;   //Evitar duplo uso do I2C (porque LCD usa DMA)




#endif /* INT_H_ */
