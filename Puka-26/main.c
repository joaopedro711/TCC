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

char sel_modo(void);

int choose;

int main(void){
    unsigned int cont=0;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    liga_perifericos();

    //config_wifi();

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
//        Vou chamar a rotina de repouso
        sel_estado();
/*       ser1_str("Digite 1 para modo de teste de modos\n");
        ser1_str("Digite 2 para modo de execucao do programa final\n");
        lcdb_apaga();
        lcdb_str(1,1,"Modo de Operacao:");
        choose=sel_modo();
        switch(choose){
        case 1:
        lcdb_apaga();
        lcdb_str(1,1,"Puka:");
        ser1_str("\r\n==> Puka: ");
        //lcd_atualiza();
        //modo=21;                    //<==Pular direto para um modo
        modo=sel_modo();          //<==Selecionar modo
        ser1_char('[');
        ser1_dec8u(modo);
        ser1_char(']');
        //modo=1;     //for�ar um dos modos
        lcdb_apaga();
        switch(modo){
            case MODO_0:   modo_0(modo);  break;
            case MODO_1:   modo_1(modo);  break;
            case MODO_2:   modo_2(modo);  break;
            case MODO_3:   modo_3(modo);  break;
            case MODO_4:   modo_4(modo);  break;
            case MODO_5:   modo_5(modo);  break;
            case MODO_6:   modo_6(modo);  break;
            case MODO_7:   modo_7(modo);  break;
            case MODO_8:   modo_8(modo);  break;
            case MODO_9:   modo_9(modo);  break;
            case MODO_10:  modo_sw(modo); break;    //Testar chaves
            case MODO_11:  modo_leds(modo);  break; //Testar Leds
            case MODO_12:  modo_lcd(modo);  break;  //Testar LCD
            case MODO_13:  modo_mpu(modo);  break;  //Testar MPU
            case MODO_14:  modo_rtc(modo);  break;  //Testar RTC
            case MODO_15:  modo_wq(modo);  break;   //Testar WQ
            case MODO_16:  modo_gps(modo);  break;  //Testar GPS
            case MODO_17:  modo_gps_d(modo);  break; //GPS em uma linha
            case MODO_18:  modo_configrtc(modo);  break;
            case MODO_19:  modo_gps_c(modo);  break; //Filtro GPS
            case MODO_20:  modo_gprmc(modo);  break;
            case MODO_21:  modo_gprs(modo);  break;
            case MODO_22:  modo_save_char(modo);  break;
            case MODO_23:  modo_save(modo);  break;
            case MODO_24:  send_wifi(modo);  break;
            case MODO_25:  modo_save_f(modo);  break;
            case MODO_26:  modo_furto(modo);  break;
            case MODO_27:    break;
            case MODO_99:  modo_99(modo);  break;   //Comando errado
        }
        break;
        case 2:
        //AA:MM:DD HH:MM:SS +LL.LLLL +LL.LLLL +axaxax +ayayay +azazaz +gxgxgx +gygygy +gzgzgz CR LF
        //lcdb_apaga();
        //modo_furto(26);*/
//        delay_10ms(42000); //atraso 7 minutos
//        while(TRUE){
//        lcdb_apaga();
//        modo_save_f(25);
//        }
/*        break;

        default: break;*/
//    }
    }
    return 0;
}
void liga_perifericos(void){
    P8DIR |= BIT2;
    P8OUT &= ~BIT2;
    P8OUT |= BIT2;
}
void toggle_consumo(void){
    P8OUT ^= BIT2;
}
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

