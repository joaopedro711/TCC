// Puka 19/07/2023
//
#include <msp430.h>
#include "Defines.h"
#include "Globais.h"
#include "Timer.h"
#include "Gpio.h"
#include "Lcd.h"
#include "Mpu.h"
#include "Wq.h"
#include "Serial.h"
#include "Strings.h"
#include "Modos.h"
#include "Gprs.h"
#include "srcs/Estados.h"

void liga_perifericos(void);

int main(void){
    unsigned int cont=0;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    liga_perifericos();

    clk_20mhz();
    crono_inic();           //Inicializar cron�metro
    crono_zera();           //Zerar cron�metro
    ta2_config();

    __enable_interrupt();

    gpio_config();

    ser1_config(BR_115200);
    ser0_config(BR_115200);

    seri_config();          //Configurar fila de entrada
    spi_config();

    //led_VM();
    //sw1_estado=ABERTA;
    //while(sw1_estado==ABERTA);  //Evitar problemas com o LCD, remover no futuro
    //led_vm();

/*    while(TRUE){
        toggle_consumo();
        __delay_cycles(20000000);
    }*/
// -------------------------------------- LCD -----------------------------------------------------------------------------------------

    i2c_config(SCL_400k);   //Usar depois do ser1_config()
    //Checar LCD e imprimir endere�o
    if (lcd_presente()==TRUE){  //Verificar o LCD
        ser1_str("\nLCD=");
        ser1_hex8(lcd_adr);
        ser1_str("\n\r");
        lcd_inic();
        lcd_dma_config();
        //ser1_str("  tem_lcd=");   ser1_dec8unz(lcd_tem);
        //ser1_str("  i2c_tranca=");   ser1_dec8unz(i2c_tranca);
    }
    else    ser1_str("\nSem LCD!");

// ------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------- MPU -----------------------------------------------------------------------------------------

    // MPU
    //delay_10ms(100);
    mpu_config();
    if (mpu_tem==FALSE) ser1_str("\nSem MPU!");
    delay_10ms(100);
// ------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------- Estados -----------------------------------------------------------------------------------------
    estados_config();
// ------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------- LOOP principal -----------------------------------------------------------------------------------------

    while(TRUE){
        //vai checar toda vez que iniciar ou der resete
       check_estado_address();

    }
    return 0;
}
// ------------------------------------------------------------------------------------------------------------------------------------

void liga_perifericos(void){
    P8DIR |= BIT2;
    P8OUT &= ~BIT2;
    P8OUT |= BIT2;
}
void toggle_consumo(void){
    P8OUT ^= BIT2;
}

//Não usei, codigo do pessoal anterior, util para aproveitar o codigo e fazer outras coisas
// Selecionar o modo
char sel_modo(void){
    char x;
    char argc[4],argv[10];
    while(TRUE){
        //ser1_str("\n==> Modo?");
        //seri_come_crlf();
        while (seri_cmdo(argc, argv, 5)==0);
        if (argv[0]=='?'){
            ser1_crlf(1);
            for (x=0; x<MODO_TOT; x++){
                //while (sero_uso()>60);
                ser1_str(modos_lista[x]);
            }
        }
        else{
            x=str_2_num(argv);
            if (x>=0 && x<MODO_TOT)  break;
            else  x=99;              break;
        }
    }
    return x;
}

