// Puka - Estados.c

#include <msp430.h>
#include "Estados.h"
#include "Gprs.h"
#include "Defines.h"
#include "Serial.h"

void estados_config(){
    estado_i=0;
    estado_y=0;
}


//Analisar comando recebido do ESP32, vou fazer um vetor receber cada caractere, então analisar qual a mensagem recebida
void sel_estado(){
    char x;

    if (gprs_tira(&x)==TRUE){
        if (x == '\n'){
            estado_y=1, estado_i=0;                 // estado_y==1 --> Posso selecionar o estado, pois já recebi o comando
        }
        else{
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

void resete(){
   // printa na serial gprs
    gprs_char('$');              //demarca o inicio de texto
    gprs_str("resete\r\n");
    gprs_char(';');              //demarca o final de texto
}

void dormente(){
    gprs_str("dormente\r\n");
    gprs_str("dormente\r\n");
}

void vigilia(){
    gprs_str("vigilia\r\n");
    gprs_str("vigilia\r\n");
}

void alerta_1(){
    gprs_str("alerta_1\r\n");
    gprs_str("alerta_1\r\n");
}

void alerta_2(){
    gprs_str("alerta_2\r\n");
    gprs_str("alerta_2\r\n");
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

void code_erro(){
    gprs_str("Deu ruim, mande novamente o comando do estado desejado!\r\n");
    gprs_str("----------------------------------\r\n");
}
