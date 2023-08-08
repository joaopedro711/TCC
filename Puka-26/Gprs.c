// GPRS.c

#include <msp430.h>
#include "Defines.h"
#include "Gprs.h"
#include "Strings.h"
//#include "Globais.h"                              //Estava dando erro ao deixar descomentada, dava conflito das variaveis globais
#include "Serial.h"
#include "srcs/Estados.h"

// Escrever 16 bits em Decimal, com sinal e com zeros à esq
void gprs_dec16(int dt){
  char msg[7];
  str_dec16(dt, msg);
  gprs_str(msg);
}

// Receber uma linha de comando da fila de entrada (gprs)
char gprs_cmdo(char *argc, char *argv, char limite){
  char x,qtd,i;
  char estado;      //modo=1, recebendo string
  i=0;
  estado=0;
  qtd=0;
  argc[0]=0;
  argv[0]='\0';
  gprs_come_crlf();
  if (gprs_vazia() == TRUE)  return qtd; //Fila vazia?
  while(TRUE){
    while (gprs_tira(&x)==FALSE);
    if (estado == 1){   //Receber um argumento
      if (x=='\n' || x=='\r')        break;
      if (x==' '){
        argv[i++]='\0';
        estado=0;
      }
      else  argv[i++]=x;
    }
    else{             //Retirar espaços
      if (x=='\n' || x=='\r') break;
      if (x != ' '){
        argc[qtd]=i;
        argv[i++]=x;
        qtd++;
        estado=1;
      }
    }
    if (i==limite){
      i--;
      break;
    }
  }
  argv[i++]='\0';
  gprs_come_crlf();
  return qtd;
}

// Verificar se a fila GPRS está vazia
// TRUE = vazia e FALSE = não vazia
char gprs_vazia(void){
    char pin, pout;
    __disable_interrupt();
    pin=gprs_pin;   //Cópia dos ponteiros
    pout=gprs_pout; //sem interrupções
    __enable_interrupt();
    pout=pout+1;
    if (pout == GPRS_FILA_TAM)    pout=0;
    if (pout == pin)  return  TRUE;   //Vazia
    else              return  FALSE;  //Tem alguma coisa na fila
}

//Consumir CR e LF
void gprs_come_crlf(void){
    char x;
    while (TRUE){
        if (gprs_xereta(&x) == FALSE) break;
        if ( x==CR || x==LF) gprs_tira(&x);
        else                  break;
    }
}

// Informa qual o próximo byte da fila GPRS
// Não altera estado dos ponteiros
// Chamada fora das interrupções
// FALSE = fila vazia
char gprs_xereta(char *cha){
  int pin,pout;
  __disable_interrupt();
  pout=gprs_pout; //Copiar
  pin=gprs_pin;   //ponteiros
  __enable_interrupt();
  pout++;
  if (pout == GPRS_FILA_TAM)    pout=0;
  if (pout == pin){
    return FALSE;
  }
  else{
    *cha=gprs_fila[pout];
    return TRUE;
  }
}

// Imprimir uma string na USCI_A0
// Escreve no padrão certo de envio de mensagem do Arapuka #---------#
void gprs_complete_str(char *msg){
    int i=0;
    gprs_char('#');
    while ( msg[i] != '\0'){
        gprs_char(msg[i++]);
    }
    gprs_char('#');
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////Modifiquei até aqui //////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Imprimir uma string na USCI_A0
// Polling TX_IFG=1
void gprs_str(char *msg){
    int i=0;
    while ( msg[i] != '\0'){
        gprs_char(msg[i++]);
    }
}

// Imprimir um caracter na USCI_A0
// Polling TX_IFG=1
void gprs_char(char x){
    while( (UCA0IFG&UCTXIFG) == 0);
    UCA0TXBUF=x;
}

////////////////////////////////////////////////////
////////////////// Fila GPRS ///////////////////////
///////////// USCI_A0 <== GPRS /////////////////////
/////// Interrupção chama gprs_poe() ///////////////
/////// Ler a fila como gprs_tira()  ///////////////
////////////////////////////////////////////////////


// Inicializar fila GPRS (USCI_A0)
void gprs_config(void){
  gprs_pin=1;
  gprs_pout=0;
}

// Colocar um byte na fila GPRS
// Chamada dentro Interrupções de RX USCI_A0
char gprs_poe(char cha){
  if (gprs_pin == gprs_pout){
    gprs_cheia();     //Msg de fila cheia
    return FALSE;     //SERI cheia
  }
  else{
    gprs_fila[gprs_pin++]=cha;
    if (gprs_pin==GPRS_FILA_TAM)
      gprs_pin=0;
    return TRUE;
  }
}

// Mensagem de Fila GPRS Cheia
void gprs_cheia(void){
  __disable_interrupt();
  ser1_str("\nERRO: GPRS Cheia!\n");
  seri_dump();
  ser1_str("\nTravado!");
  while(TRUE);
}

// Tirar um byte da fila GPRS
// Função chamada fora das interrupções
char gprs_tira(char *cha){
  int pout_aux;
  __disable_interrupt();
  pout_aux=gprs_pout+1;
  if (pout_aux == GPRS_FILA_TAM)    pout_aux=0;
  if (pout_aux == gprs_pin){
    __enable_interrupt();
    return FALSE;
  }
  else{
    *cha=gprs_fila[pout_aux];
    gprs_pout=pout_aux;
    __enable_interrupt();
    return TRUE;
  }
}

// USCI_A0: Configurar Serial 1 (MSP <=> GPRS), Hab int RX
// Usa SMCLK = 20 MHz
// P3.3 = TX (MSP ==> GPRS) e P3.4 = RX (MSP <== GPRS)
void ser0_config(char br){
    UCA0CTL1 =  UCSSEL_2 | UCSWRST;     //UCSI em Reset
    UCA0CTL0 = UCSPB;                   //1 Stop

    switch(br){
        case BR_4800:
        case BR_9600:    UCA0BRW = 2083;  UCA0MCTL=UCBRS_2|UCBRF_0;  break;
        case BR_19200:   UCA0BRW = 1041;  UCA0MCTL=UCBRS_6|UCBRF_0;  break;
        case BR_38400:   UCA0BRW = 520;   UCA0MCTL=UCBRS_7|UCBRF_0;  break;
        case BR_57600:   UCA0BRW = 347;   UCA0MCTL=UCBRS_2|UCBRF_0;  break;
        case BR_74880:
        case BR_115200:  UCA0BRW = 173;   UCA0MCTL=UCBRS_5|UCBRF_0;  break;
        case BR_230400:  UCA0BRW = 86;    UCA0MCTL=UCBRS_7|UCBRF_0;  break;
        case BR_250k:    UCA0BRW = 80;    UCA0MCTL=0;                break;
        case BR_500k:    UCA0BRW = 40;    UCA0MCTL=0;                break;
        case BR_1000k:   UCA0BRW = 20;    UCA0MCTL=0;                break;
        case BR_2000k:   UCA0BRW = 10;    UCA0MCTL=0;                break;
        default:         UCA0BRW = 2083;  UCA0MCTL=UCBRS_2|UCBRF_0;  break; //default = 9600 bps

    }
    P3DIR |=  BIT3;         //P3.3 saída
    P3DIR &= ~BIT4;         //P3.4 entrada
    P3SEL |= BIT4 | BIT3;   //Tx serial

    UCA0CTL1 &= ~ UCSWRST;  //UCSI sai de Reset
    //UCA0IE = UCTXIE|UCRXIE; //Hab interrupção  TX e RX
    UCA0IE = UCRXIE;            //Hab interrupção  RX
}


// USCI_A0 - ISR (GPRS <==> USCI_A0)
#pragma vector = 56
__interrupt void ISR_USCI_A0(void){
    int n;
    n = __even_in_range(UCA0IV,0x4);
    switch(n){
        case 0x0: break;
        case 0x2: //RX
            gprs_poe(UCA0RXBUF);
            break;
        case 0x4: break; //TX
    }

}







