// Puka - Estados.c

#include <msp430.h>
#include "Estados.h"
#include "Gprs.h"
#include "Defines.h"
#include "Serial.h"
#include "Modos.h"

void estados_config(){
    estado_i=0;
    estado_y=0;
}


//Analisar comando recebido do ESP32, vou fazer um vetor receber cada caractere, então analisar qual a mensagem recebida
void sel_estado(){
    char x;

//    if (gprs_tira(&x)==TRUE){
//        if (x == '\n'){
//            estado_y=1, estado_i=0;                 // estado_y==1 --> Posso selecionar o estado, pois já recebi o comando
//        }
//        else if(x >= ' ' && x <= 'z'){              //garantir de receber apenas o que realmente quero, estava vindo caracteres loucos
//            estado_comando[estado_i++] = x;         //armazena os caracteres dentro de estado_comando
//        }
//    }
    while (gprs_tira(&x)==TRUE){
         if (x == '\n'){
             estado_y=1, estado_i=0;                 // estado_y==1 --> Posso selecionar o estado, pois já recebi o comando
         }
         else if(x >= ' ' && x <= 'z'){              //garantir de receber apenas o que realmente quero, estava vindo caracteres loucos
             estado_comando[estado_i++] = x;         //armazena os caracteres dentro de estado_comando
         }
     }

    if(estado_y == 1){
        estado_y = 0;
        if(estado_comando[0] == '#' && estado_comando[1] == 'R' && estado_comando[2] == 'S' && estado_comando[3] == 'T' && estado_comando[4] == '#'){
            resete();
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'D' && estado_comando[2] == 'M' && estado_comando[3] == 'T' && estado_comando[4] == '#'){
            dormente();
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'V' && estado_comando[2] == 'I' && estado_comando[3] == 'G' && estado_comando[4] == '#'){
             vigilia();
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'A' && estado_comando[2] == 'L' && estado_comando[3] == 'T' && estado_comando[4] == '1' && estado_comando[5] == '#'){
            alerta_1();
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'A' && estado_comando[2] == 'L' && estado_comando[3] == 'T' && estado_comando[4] == '2' && estado_comando[5] == '#'){
            alerta_2();
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'S' && estado_comando[2] == 'P' && estado_comando[3] == 'T' && estado_comando[4] == '#'){
            suspeito();
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'A' && estado_comando[2] == 'P' && estado_comando[3] == 'G' && estado_comando[4] == '#'){
            apagar();
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'R' && estado_comando[2] == 'T' && estado_comando[3] == 'C'){
             rtc_configure();
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'R' && estado_comando[2] == 'D' && estado_comando[3] == ' ') { //Checa se é o comando de leitura das posições de memoria
            if(estado_comando[4] == 'n' && estado_comando[5] == '#'){
                ler_n();
            }
            else if (estado_comando[4] == 'n' && estado_comando[5] == ' ' && estado_comando[6] == 'm' && estado_comando[7] == '#'){
                ler_n_m();
            }
            else{
                code_erro();
            }
        }
        else{
            code_erro();
        }
    }

}

// Realiza o resete do MSP por software
void resete(){
    PMMCTL0 = PMMPW | PMMSWPOR; // Configuração para realizar um software reset
}

void dormente(){
    while(TRUE){
        sel_estado();

        rtc_estado();
        gps_estado_modo();
        todos_dados();
        gprs_complete_str(toda_msg);
        gprs_complete_str("\n\r---------------\n\r");
        delay_10ms(200); //1 seg
    }
}

void vigilia(){
    set_values_mpu();

    while(TRUE){
        if(acel_furto()==FALSE){
            gprs_str("OK\r\n");
        }
        else if(acel_furto()==TRUE){
            gprs_str("FURTADO\r\n");
        }
        delay_10ms(100); //0,5 seg
    }
}

void alerta_1(){
    //gps_furto();
    set_values_gps();
    ser1_char(' ');ser1_dec32u(menorlatitude);ser1_char(' '); ser1_dec32u(maiorlatitude); ser1_str(" ---- ");
    delay_10ms(100); //0,5 seg
    ser1_dec32u(menorlongitude); ser1_char(' ');ser1_dec32u(maiorlongitude); ser1_str("\n\r");
    delay_10ms(100); //0,5 seg
    while(1){
        if(gps_furto() == FALSE){
            gprs_complete_str("OK");
            ser1_dec32u(lat);ser1_str("   "); ser1_dec32u(longt); ser1_str("\n\r");
        }
        else if(gps_furto() == TRUE) gprs_complete_str("FURTADO");
        delay_10ms(200); //1 seg
    }
}

void alerta_2(){
    rtc_estado();
    atualiza_data_hora(TRUE, TRUE);
    while(TRUE){
        rtc_estado();
       // gprs_str(rtc_msg);
        //gprs_str("\n\r");
        if(passou_1_min()==TRUE){

            ///envia posição do gps

            atualiza_data_hora(TRUE, FALSE); //atualiza apenas o minuto
            gprs_str(rtc_msg);
            gprs_str("\n\r----- passou 1 min -------\n\r");

            //pega dados do gps
            gps_estado_modo();
            //junta todos os dados necessarios em apenas uma string
            todos_dados();
            gprs_str(toda_msg);
        }
        if(passou_1_hora()==TRUE){

            ////envia posição do gps e grava na memoria
            atualiza_data_hora(FALSE, TRUE); //atualiza apenas a hora
            gprs_str(rtc_msg);
            gprs_str("\n\r----- passou 10 seg -------\n\r");
        }
    }
}

void suspeito(){
    gprs_str("suspeito\r\n");
    gprs_str("suspeito\r\n");
}

void apagar(){
    gprs_str("apagar\r\n");
    gprs_str("apagar\r\n");
}

void ler_n(){
    gprs_str("ler_100\r\n");
    gprs_str("ler_100\r\n");
}

void ler_n_m(){
    gprs_str("ler_n_m\r\n");
    gprs_str("ler_n_m\r\n");
}

//Seta horario por gprs
void rtc_configure(){
    char vetor[3];

    //Data (dd/mm/aa)
    vetor[0]=16*(estado_comando[5]-0x30)+(estado_comando[6]-0x30);  //Dia
    vetor[1]=16*(estado_comando[8]-0x30)+(estado_comando[9]-0x30);  //Mï¿½s
    vetor[2]=16*(estado_comando[11]-0x30)+(estado_comando[12]-0x30);  //Ano
    rtc_wr_vet(4,vetor,3);

    // Hora (hh:mm:ss)
    vetor[2]=16*(estado_comando[20]-0x30)+(estado_comando[21]-0x30);  //Segundos
    vetor[1]=16*(estado_comando[17]-0x30)+(estado_comando[18]-0x30);  //Minutos
    vetor[0]=16*(estado_comando[14]-0x30)+(estado_comando[15]-0x30);  //Horas
    rtc_wr_vet(0,vetor,3);
}

//Precisa ser acertado
//Coloca ou tira do baixo consumo
void baixo_consumo(){
    //coloca em baixo consumo
    // Entrada do LPM0

   __bis_SR_register(LPM0_bits | GIE); // Entra em LPM0 com interrupções habilitadas


    //retira do baixo consumo
    // Saída do LPM0

        //não pode estar aqui, precisa estar dentro da interrção desejada
//        __bic_SR_register_on_exit(LPM0_bits); // Remove o bit de LPM0 do registrador de status e sai do LPM0

}

void code_erro(){
    gprs_str("Deu ruim, mande novamente o comando do estado desejado!\r\n");
    gprs_str("----------------------------------\r\n");
}
