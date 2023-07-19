// Puka - Timer.h

#ifndef TIMER_H_
#define TIMER_H_

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
extern volatile unsigned int led_vm_padrao; //Padr�o para o pisca-pisca do led vermelho
extern volatile unsigned int led_vd_padrao; //Padr�o para o pisca-pisca do led verde


unsigned long crono_ler(void);
void crono_inic(void);
void crono_start(void);
void crono_stop(void);
void crono_zera(void);
unsigned char crono_calibra(void);

void delay_seg(int dur);
void delay_10ms(int dur);
void ta2_config(void);

void clk_20mhz(void);


#endif /* TIMER_H_ */
