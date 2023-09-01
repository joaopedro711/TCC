// Puka - Estados.c

#include <msp430.h>
#include "Estados.h"
#include "Gprs.h"
#include "Defines.h"
#include "Serial.h"
#include "Modos.h"
#include "Timer.h"
#include "Rtc.h"
#include "Wq.h"

void estados_config(){
    estado_i=0, estado_y=0;
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
            ler_memoria();                                          // Funcao que decide se vai ler_n ou ler_n_m
            //Significa que eh o comando de ler n primeiras posições
//            if(estado_comando[5] == '#'){
//                ler_n();
//            }
//            //Significa que eh o comando de ler n m posições (#RD n m#)
//            else if (estado_comando[5] == ' ' &&  estado_comando[7] == '#'){
//                ler_n_m();
//            }
//            else{
//                code_erro();
//            }
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'M' && estado_comando[2] == 'A' && estado_comando[3] == 'I' && estado_comando[4] == 'L' && estado_comando[5] == '#'){
            email();
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'S' && estado_comando[2] == 'T' && estado_comando[3] == 'A' && estado_comando[4] == 'T' && estado_comando[5] == '#'){
            status();
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
    ler_memoria_estado();
    estado_puka[0]= 'D',estado_puka[1]= 'M',estado_puka[2]= 'T',estado_puka[3]= ' ', estado_puka[4]= '-', estado_puka[5]= ' ',estado_puka[6]= '\0';
    salvar_ultimo_estado_address_memoria();
    gprs_complete_str(estado_puka);
    gprs_complete_str("Arapuka em estado Dormente");

    while(TRUE){
        sel_estado();

//        rtc_estado();
//        gps_estado_modo();
//        todos_dados(FALSE);
//        gprs_complete_str(toda_msg);
//        gprs_complete_str("\n\r---------------\n\r");
//        delay_10ms(200); //1 seg
    }
}

void vigilia(){
    estado_puka[0]= 'V',estado_puka[1]= 'I',estado_puka[2]= 'G',estado_puka[3]= ' ', estado_puka[4]= '-', estado_puka[5]= ' ',estado_puka[6]= '\0';
    gprs_complete_str(estado_puka);
    gprs_complete_str("Arapuka em estado de Vigilia");
    salvar_ultimo_estado_address_memoria();
    //definindo os valores do repouso do Arapuka
    repouso_values_mpu();
    set_values_gps();

    while(TRUE){
        sel_estado();

        //se foi furtado entra em Alerta 1
        if(acel_furto()==TRUE || giro_furto()==TRUE || gps_furto()==TRUE){
            alerta_1();
        }
    }
}

// Envia posição a cada hora e grava na memoria
void alerta_1(){
    estado_puka[0]= 'A',estado_puka[1]= 'L',estado_puka[2]= 'T',estado_puka[3]= '1',estado_puka[4]= ' ', estado_puka[5]= '-', estado_puka[6]= ' ',estado_puka[7]= '\0';
    gprs_complete_str(estado_puka);
    gprs_complete_str("Arapuka em estado de Alerta 1");
    salvar_ultimo_estado_address_memoria();

    rtc_estado();
    atualiza_data_hora(FALSE, TRUE);            //Só preciso atualizar a ultima hora
    while(TRUE){
        sel_estado();
        rtc_estado();


        if(passou_1_hora()==TRUE){
            atualiza_data_hora(FALSE, TRUE);            //Só preciso atualizar a ultima hora
            gps_estado_modo();
            todos_dados(TRUE);
            gprs_complete_str(toda_msg);

            //SALVA NA MEMORIA
            //
            //
        }
    }
}

// Envia posição a cada min, a cada hora grava na memoria
void alerta_2(){
    estado_puka[0]= 'A',estado_puka[1]= 'L',estado_puka[2]= 'T',estado_puka[3]= '2',estado_puka[4]= ' ', estado_puka[5]= '-', estado_puka[6]= ' ',estado_puka[7]= '\0';
    gprs_complete_str(estado_puka);
    gprs_complete_str("Arapuka em estado de Alerta 2");
    salvar_ultimo_estado_address_memoria();

    rtc_estado();
    atualiza_data_hora(TRUE, TRUE);            //Só preciso atualizar a ultima hora
    while(TRUE){
        sel_estado();
        rtc_estado();

        if(passou_1_min()==TRUE){
            atualiza_data_hora(TRUE, FALSE); //atualiza apenas o minuto

            gps_estado_modo();
            todos_dados(TRUE);
            gprs_complete_str(toda_msg);
        }

        if(passou_1_hora()==TRUE){
            atualiza_data_hora(FALSE, TRUE);            //Só preciso atualizar a ultima hora
            gps_estado_modo();
            todos_dados(TRUE);
            gprs_complete_str(toda_msg);

            //SALVA NA MEMORIA
            //
            //
        }
    }
}

void suspeito(){
   estado_puka[0]= 'S',estado_puka[1]= 'P',estado_puka[2]= 'T',estado_puka[3]= ' ', estado_puka[4]= '-', estado_puka[5]= ' ',estado_puka[6]= '\0';
////    rtc_estado();
////    gps_estado_modo();
////    //argumento FALSE para receber apenas os valores do RTC + GPS
////    todos_dados(TRUE);
   gprs_complete_str(estado_puka);
   salvar_ultimo_estado_address_memoria();
   gprs_complete_str(estado_puka);
////    gprs_complete_str(toda_msg);
////    delay_10ms(200);
//    int i;
//    long wr_adr=0, rd_adr = 0;
//    char vetor[7];
//    char vt[16];
//    char msg[48];
//    msg[0]= estado_puka[0], msg[1]= estado_puka[1], msg[2]= estado_puka[2], msg[3]= estado_puka[3], msg[4]= ' ';
//    msg[5]= '1', msg[6]= '0', msg[7]= '2', msg[8]= '4', msg[9]= '0', msg[10]= '0', msg[11]= '0';
//
////    msg[0]= estado_puka[0], msg[1]= estado_puka[1], msg[2]= estado_puka[2], msg[3]= estado_puka[3], msg[4]= ' ';
////    msg[5]= 'FF', msg[6]= 'FF', msg[7]= 'FF', msg[8]= 'FF', msg[9]= 'FF', msg[10]= 'FF', msg[11]= 'FF';
//
//    for(i=12; i<48; i++){
//        msg[i]= ' ';
//    }
//    //Antes de escrever precisa apagar a memoria
//    wq_erase_4k(0);
//    //salvar nas 48 primeiras posições
//    for(i = 0; i<48; i++){
//        save_data(msg[i], wr_adr + i);
//    }
//    //Ler da memoria
//    //chama a funçao de ler memoria
//    // ler apenas os primeiros 16 registros
//    for (i=0; i<i; i++){
//       wq_rd_blk(rd_adr, vt,128);
//       ser1_hex32(rd_adr);
//       ser1_char(':');
//       ser1_spc(1);
//       ser1_linha(vt);
//       //rd_adr+=16;
//    }
//   int y=0;
//  // estado_puka[0]= 'D',estado_puka[1]= 'M',estado_puka[2]= 'T',estado_puka[3]= ' ', estado_puka[4]= '-', estado_puka[5]= ' ',estado_puka[6]= '\0';
//
//    ler_memoria_estado();
//    gprs_complete_str(mem_vetor);
//    salvar_ultimo_estado_address_memoria();
//    ler_memoria_estado();
//    gprs_complete_str(mem_vetor);
}

void apagar(){
    wq_erase_chip();
    gprs_complete_str("Toda Memoria apagada");
}

//Decide qual função chamar, se chama a de ler_n ou ler_n_m
void ler_memoria(){
    unsigned int i,z,entrou=0;
    int n=0, m=0,j=1;

    //Ao percorrer os caracteres, checar quem encontra primeiro
    for(i=4; i<24;i++){
        //ler_n
        if(estado_comando[i]=='#' && entrou == 0){
            i--;
            while(i>3){
                n+= (estado_comando[i]-'0') * j;
                i--;
                j*=10;
            }
            ler_n(n);               //passa posição do fim do vetor
            entrou=1;
        }
        //Ler_n_m
        else if(estado_comando[i]==' ' && entrou == 0){
            ///// n
            z=i;    //onde esta o caractere de espaco
            i--;
            while(i>3){
                n+= (estado_comando[i]-'0') * j;
                i--;
                j*=10;
            }

            ///// m
            i=z,j=1;
            while(estado_comando[i] != '#') i++;
            i--;
            while(i>z){
                m+= (estado_comando[i]-'0') * j;
                i--;
                j*=10;
            }
            ler_n_m(n,m);
            entrou=1;
        }
    }
}

void ler_n(int n){
    gprs_complete_str("n");
   // gprs_str("ler_100\r\n");
}

void ler_n_m(int n,int m){
    gprs_complete_str("n");
    gprs_complete_str("m");
}

//recebe horario por gprs e salva no RELOGIO
void rtc_configure(){
    char vetor[3];

    //Data (dd/mm/aa)
    vetor[0]=16*(estado_comando[5]-0x30)+(estado_comando[6]-0x30);  //Dia
    vetor[1]=16*(estado_comando[8]-0x30)+(estado_comando[9]-0x30);  //Mes
    vetor[2]=16*(estado_comando[11]-0x30)+(estado_comando[12]-0x30);  //Ano
    rtc_wr_vet(4,vetor,3);

    // Hora (hh:mm:ss)
    vetor[2]=16*(estado_comando[14]-0x30)+(estado_comando[15]-0x30);  //Horas
    vetor[1]=16*(estado_comando[17]-0x30)+(estado_comando[18]-0x30);  //Minutos
    vetor[0]=16*(estado_comando[20]-0x30)+(estado_comando[21]-0x30);  //Segundos
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
    gprs_complete_str("ERROR, comando invalido.");
}

// Envia RTC + GPS (conteúdo do email)
void email(){
    rtc_estado();
    gps_estado_modo();
    //argumento FALSE para receber apenas os valores do RTC + GPS
    todos_dados(FALSE);
    gprs_complete_str(toda_msg);
}

// retorna todos os dados para o ESP32
//envia o estado atual, data/hora, localização e acel/gir
void status(){
    rtc_estado();
    gps_estado_modo();
    //argumento FALSE para receber apenas os valores do RTC + GPS
    todos_dados(TRUE);
    gprs_complete_str(estado_puka);                                 //Envia o estado atual
    gprs_complete_str(toda_msg);
}
