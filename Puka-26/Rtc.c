// Puka - Rtc.c
// I2C (USCI_B1) P4.2 = SCL
// I2C (USCI_B1) P4.1 = SDA

// Endere�o do RTC = 0x68
// Endere�o do MPU = 0x68+1

#include <msp430.h>
//#include "Defines.h"
//#include "Timer.h"
#include "Lcd.h"
#include "Serial.h"
#include "Rtc.h"

//separar caracteres e armazenar em um vetor
void rtc_str(char *msg){
    int i = 0;
    while ( msg[i] != '\0'){
        vetor_rtc[i] = msg[i];
        i++;
    }
}

//avan�ar quantidade "qtd" de v�rgulas
char rtc_come_vig(char ini, char *vt, char qtd){
    char cont, ix=ini;
    cont = 0;
    while (cont<qtd){
      while( vt[ix] != ','){
        ix++;
      }
      ix++;
      cont++;
    }
    return ix;
}
// Escrever uma sequ�ncia de dados no RTC
void rtc_wr_vet(char reg, char *vt, char qtd){
    char i;
    if (i2c_trancar()==FALSE){
        ser1_str("\nRTC nao tracou I2C.");
        while(1);
    }
    UCB1I2CSA = RTC_ADR;                //Endere�o do MPU

    // Indicar registrador de onde come�a a leitura
    UCB1CTL1 |= UCTR | UCTXSTT;                 //Mestre TX + Gerar START
    while ( (UCB1IFG & UCTXIFG) == 0);          //Esperar TXIFG=1
    UCB1TXBUF = reg;                            //Escrever registrador
    while ( (UCB1CTL1 & UCTXSTT) == UCTXSTT);   //STT=0?
    if ( (UCB1IFG & UCNACKIFG) == UCNACKIFG){   //NACK?
        P1OUT |= BIT0;                          //NACK=problema
        while(1);                               //Travar execu��o
    }

    // Escrever a quantidade de dados
    for (i=0; i<qtd; i++){
        while ((UCB1IFG & UCTXIFG) == 0);   //Esperar TX
          UCB1TXBUF=vt[i];                  //Escrever dado
    }
    while ((UCB1IFG & UCTXIFG) == 0);   //Esperar TX
    // Gerar STOP
    UCB1CTL1 |= UCTXSTP;                           //Gerar STOP
    while ( (UCB1CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    i2c_tranca=FALSE;
}



// Ler sequ�ncia de dados do RTC
void rtc_rd_vet(char reg, char *vt, char qtd){
    char i;
    if (i2c_trancar()==FALSE){
        ser1_str("\nRTC nao tracou I2C.");
        while(1);
    }
    UCB1I2CSA = RTC_ADR;                //Endere�o do MPU

    // Indicar registrador de onde come�a a leitura
    UCB1CTL1 |= UCTR | UCTXSTT;                 //Mestre TX + Gerar START
    while ( (UCB1IFG & UCTXIFG) == 0);          //Esperar TXIFG=1
    UCB1TXBUF = reg;                            //Escrever registrador
    while ( (UCB1CTL1 & UCTXSTT) == UCTXSTT);   //STT=0?
    if ( (UCB1IFG & UCNACKIFG) == UCNACKIFG){   //NACK?
        P1OUT |= BIT0;                          //NACK=problema
        while(1);                               //Travar execu��o
    }

    // Configurar escravo transmissor
    UCB1CTL1 &= ~UCTR;         //Mestre RX
    UCB1CTL1 |= UCTXSTT;       //Gerar START Repetido
    while ( (UCB1CTL1 & UCTXSTT) == UCTXSTT);   //STT=0?

    // Ler a quantidade de dados, menos o �ltimo
    for (i=0; i<qtd-1; i++){
    while ((UCB1IFG & UCRXIFG) == 0);       //Esperar RX
          vt[i]=UCB1RXBUF;                  //Ler dado
    }

    // Ler o �ltimo dado e gerar STOP
    UCB1CTL1 |= UCTXSTP;                           //Gerar STOP
    while ((UCB1IFG & UCRXIFG) == 0);              //Esperar RX
    vt[i]=UCB1RXBUF;                               //Ler dado
    while ( (UCB1CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    i2c_tranca=FALSE;
}

// Ler um registrador do RTC
char rtc_rd(char reg){
    if (i2c_trancar()==FALSE){
        ser1_str("\nRTC nao tracou I2C.");
        while(1);
    }
    UCB1I2CSA = RTC_ADR;                //Endere�o do RTC
    UCB1CTL1 |= UCTR | UCTXSTT;         //Mestre TX + Gerar START
    while ( (UCB1IFG & UCTXIFG) == 0);  //Esperar TXIFG=1
    UCB1TXBUF = reg;                    //Escrever registrador
    while ( (UCB1CTL1 & UCTXSTT) == UCTXSTT);   //STT=0?
    if ( (UCB1IFG & UCNACKIFG) == UCNACKIFG){   //NACK?
        P1OUT |= BIT0;                          //NACK=problema
        while(1);                               //Travar execu��o
    }

    // Configurar escravo transmissor
    UCB1CTL1 &= ~UCTR;         //Mestre RX
    UCB1CTL1 |= UCTXSTT;       //Gerar START Repetido
    while ( (UCB1CTL1 & UCTXSTT) == UCTXSTT);   //STT=0?
    UCB1CTL1 |= UCTXSTP;                        //Gerar STOP
    while ( (UCB1IFG & UCRXIFG) == 0);          //Esperar RX
    while ( (UCB1CTL1 & UCTXSTP) == UCTXSTP);   //Esperar STOP
    i2c_tranca=FALSE;
    return UCB1RXBUF;
}

// Escrever num registrador do RTC
void rtc_wr(char reg, char dado){
    if (i2c_trancar()==FALSE){
        ser1_str("\nRTC nao tracou I2C.");
        while(1);
    }
    UCB1I2CSA = RTC_ADR;        //Endere�o do RTC
    UCB1CTL1 |= UCTR | UCTXSTT;                 //Mestre TX + START
    while ( (UCB1IFG & UCTXIFG) == 0);          //TXIFG=1?
    UCB1TXBUF = reg;                            //Escrever dado
    while ( (UCB1CTL1 & UCTXSTT) == UCTXSTT);   //Esperar STT=0
    if ( (UCB1IFG & UCNACKIFG) == UCNACKIFG){   //NACK?
        P1OUT |= BIT0;      //NACK=Sinalizar problema
        while(1);           //Travar execu��o
    }
    while ( (UCB1IFG & UCTXIFG) == 0);  //TXIFG=1?
    UCB1TXBUF = dado;                   //Escrever dado
    while ( (UCB1IFG & UCTXIFG) == 0);  //TXIFG=1?
    UCB1CTL1 |= UCTXSTP;                       //Gerar STOP
    while ( (UCB1CTL1 & UCTXSTP) == UCTXSTP);  //Esperar STOP
    i2c_tranca=FALSE;
}


