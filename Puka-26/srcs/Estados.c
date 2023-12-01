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
    wr_address_mem = 0;
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
        else if(estado_comando[0] == '#' && estado_comando[1] == 'g' && estado_comando[2] == 'r' && estado_comando[3] == 'e' && estado_comando[7] == 'o' && estado_comando[8] == 'n'){
            led_VD();
            gprs_complete_str("LED Verde ON");
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'g' && estado_comando[2] == 'r' && estado_comando[3] == 'e' && estado_comando[7] == 'o' && estado_comando[8] == 'f' && estado_comando[9] == 'f'){
            led_Vd();
            gprs_complete_str("LED Verde Off");
        }

        else if(estado_comando[0] == '#' && estado_comando[1] == 'r' && estado_comando[2] == 'e' && estado_comando[3] == 'd' && estado_comando[5] == 'o' && estado_comando[6] == 'n'){
            led_VM();
            gprs_complete_str("Led Vermelho ON");
        }
        else if(estado_comando[0] == '#' && estado_comando[1] == 'r' && estado_comando[2] == 'e' && estado_comando[3] == 'd' && estado_comando[5] == 'o' && estado_comando[6] == 'f' && estado_comando[7] == 'f'){
            led_Vm();
            gprs_complete_str("Led Vermelho Off");
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
    estado_puka[0]= 'D',estado_puka[1]= 'M',estado_puka[2]= 'T',estado_puka[3]= ' ', estado_puka[4]= '-', estado_puka[5]= ' ',estado_puka[6]= '\0';
    gprs_complete_str(estado_puka);
    gprs_complete_str("Arapuka em estado Dormente");

    while(TRUE){
        sel_estado();
    }
}

void vigilia(){
    estado_puka[0]= 'V',estado_puka[1]= 'I',estado_puka[2]= 'G',estado_puka[3]= ' ', estado_puka[4]= '-', estado_puka[5]= ' ',estado_puka[6]= '\0';
    gprs_complete_str(estado_puka);
    gprs_complete_str("Arapuka em estado de Vigilia");
    salvar_memoria();

    //definindo os valores do repouso do Arapuka
    repouso_values_mpu();
    set_values_gps();

    while(TRUE){
        sel_estado();

        //se foi furtado entra em Alerta 1
        if(acel_furto()==TRUE || giro_furto()==TRUE || gps_furto()==TRUE){
            suspeito();
        }
    }
}

// Envia posição a cada hora e grava na memoria
void alerta_1(){
    estado_puka[0]= 'A',estado_puka[1]= 'L',estado_puka[2]= 'T',estado_puka[3]= '1',estado_puka[4]= ' ', estado_puka[5]= '-', estado_puka[6]= ' ',estado_puka[7]= '\0';
    gprs_complete_str(estado_puka);
    gprs_complete_str("Arapuka em estado de Alerta 1");
    salvar_memoria();

    gps_estado_modo();
    todos_dados(TRUE);
    gprs_complete_str(toda_msg);

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
            salvar_memoria();
        }
    }
}

// Envia posição a cada min, a cada hora grava na memoria
void alerta_2(){
    estado_puka[0]= 'A',estado_puka[1]= 'L',estado_puka[2]= 'T',estado_puka[3]= '2',estado_puka[4]= ' ', estado_puka[5]= '-', estado_puka[6]= ' ',estado_puka[7]= '\0';
    gprs_complete_str(estado_puka);
    gprs_complete_str("Arapuka em estado de Alerta 2");
    salvar_memoria();

    gps_estado_modo();
    todos_dados(TRUE);
    gprs_complete_str(toda_msg);

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
            salvar_memoria();
        }
    }
}

void suspeito(){
    estado_puka[0]= 'S',estado_puka[1]= 'P',estado_puka[2]= 'T',estado_puka[3]= ' ', estado_puka[4]= '-', estado_puka[5]= ' ',estado_puka[6]= '\0';
    gprs_complete_str(estado_puka);
    gprs_complete_str("Arapuka em estado Suspeito");
    salvar_memoria();

    gps_estado_modo();
    todos_dados(TRUE);
    gprs_complete_str(toda_msg);

    rtc_estado();
    atualiza_data_hora(FALSE, TRUE);            //Só preciso atualizar a ultima hora

    int i=0;
    while(TRUE){
        sel_estado();
        rtc_estado();

        //envia mensagem a cada 3h
        if(passou_1_hora()==TRUE){
            i++;
            if(i==3){
                atualiza_data_hora(FALSE, TRUE);            //Só preciso atualizar a ultima hora
                gps_estado_modo();
                todos_dados(TRUE);
                gprs_complete_str(toda_msg);

                //SALVA NA MEMORIA
                salvar_memoria();
                i=0;
            }
        }
    }

}

void apagar(){
    wq_erase_chip();
    gprs_complete_str("Toda Memoria apagada");
}

//Decide qual função chamar, se chama a de ler_n ou ler_n_m
void ler_memoria(){
    unsigned int i,z,entrou=0;
    int j=1;

    n_rd=0, m_rd=0;

    //Ao percorrer os caracteres, checar quem encontra primeiro
    for(i=4; i<24;i++){
        //ler_n
        if(estado_comando[i]=='#' && entrou == 0){
            i--;
            while(i>3){
                n_rd+= (estado_comando[i]-'0') * j;
                i--;
                j*=10;
            }
            entrou=1;
            ler_n();               //passa posição do fim do vetor
        }
        //Ler_n_m
        else if(estado_comando[i]==' ' && entrou == 0){
            ///// n
            z=i;    //onde esta o caractere de espaco
            i--;
            while(i>3){
                n_rd+= (estado_comando[i]-'0') * j;
                i--;
                j*=10;
            }

            ///// m
            i=z,j=1;
            while(estado_comando[i] != '#') i++;
            i--;
            while(i>z){
                m_rd+= (estado_comando[i]-'0') * j;
                i--;
                j*=10;
            }
            entrou=1;
            ler_n_m();
        }
    }
}

void ler_n(){
    unsigned int i;
    char vt[100];

    long address=0;


    //ler memoria 128 em 128 registros e mostrar no gprs
    for(i=0; i<n_rd; i++){
        wq_rd_blk(address, vt, 100);                 //ler as 128 primeiras posições da memoria
        gprs_complete_str(vt);
        address+=128;   //pula para os proximo 128 registros
        delay_10ms(400);
    }
}

void ler_n_m(int n,int m){
    unsigned int i;
    int cont;
    char vt[100];

    long address=0, wr=0;


    cont = (m_rd - n_rd) + 1;     // pega ate o registro igual a m, ou seja, pega o registro m tambem
    address = (n_rd * 128) - 128; //pega do registro n

    //ler memoria 128 em 128 registros e mostrar no gprs
    for(i=0; i<cont; i++){
        wq_rd_blk(address, vt, 100);                 //ler as 100 primeiras posições da memoria, so precisa ler isso
        gprs_complete_str(vt);
        address+=128;   //pula para os proximo 128 registros
        delay_10ms(400);
    }
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
