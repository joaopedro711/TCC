// Existem algumas funções que aproveitam funções de outros codigos, porém agora essas enviam por gprs

#include <msp430.h>
#include "Modos.h"
#include "Gpio.h"
#include "Timer.h"
#include "Serial.h"
#include "Lcd.h"
#include "Mpu.h"
#include "Rtc.h"
#include "Wq.h"
#include "Gps.h"
#include "Strings.h"
#include "Gprs.h"
#include "Estados.h"
//#include "Globais.h"

//Coloca a data e hora no vetor rtc_msg
void rtc_estado(){
    rtc_rd_vet(0, rtc_vetor, 7);
    str_data_hora(rtc_vetor,rtc_msg);
}

//Atualiza o vetor que contem a data e hora do ultimo alerta de roubo
void atualiza_data_hora(int min, int hora){
    int i;
    //atualiza minutos
    if(min == TRUE){
        for(i=0; i<18; i++){
            rtc_ultima_data_hora_1_min[i] = rtc_msg[i];
        }
    }
    //atualiza hora
    if(hora == TRUE){
        for(i=0; i<18; i++){
            rtc_ultima_data_hora_1_hora[i] = rtc_msg[i];
        }
    }
}
char passou_1_min(){
    volatile unsigned int result1, result2, result;
    coloca_min_dezena();

    //checa se é o mesmo dia/mes/ano
    if((atual_vetor_data_hora[0] == ultimo_vetor_data_hora[0]) && (atual_vetor_data_hora[1] == ultimo_vetor_data_hora[1]) && (atual_vetor_data_hora[2] == ultimo_vetor_data_hora[2])){
       //transforma horas,min, seg em segundos e subtrai
        result1 = ((ultimo_vetor_data_hora[3] * 3600) + (ultimo_vetor_data_hora[4] * 60) + ultimo_vetor_data_hora[5]);
        result2 = ((atual_vetor_data_hora[3] * 3600) + (atual_vetor_data_hora[4] * 60) + atual_vetor_data_hora[5]);

        if(result1 > result2){
            result = result1 - result2;
        }
        else if(result1 < result2){
            result = result2 - result1;
        }
        //não passou nenhum segundo, garantir que result não vai receber valores loucos
        else{
            result = 0;
        }

       //if passou 1 min
        if (result > 59){
           return TRUE;
       }
       return FALSE;
    }
   // caso não seja o mesmo dia
    else{
        //transforma horas,min, seg em segundos e subtrai
         result1 = (86400 - ((ultimo_vetor_data_hora[3] * 3600) + (ultimo_vetor_data_hora[4] * 60) + ultimo_vetor_data_hora[5]));
         result2 = ((atual_vetor_data_hora[3] * 3600) + (atual_vetor_data_hora[4] * 60) + atual_vetor_data_hora[5]);
        //if passou 1 min
         if(result1 > result2){
             result = result1 - result2;
         }
         else if(result1 < result2){
             result = result2 - result1;
         }
         //não passou nenhum segundo, garantir que result não vai receber valores loucos
         else{
             result = 0;
         }

         //if passou 1 min
         if (result > 59){
             return TRUE;
         }
         return FALSE;
    }
}

char passou_1_hora(){
    coloca_hora_dezena();
    volatile unsigned int result1, result2, result;
    //checa se é o mesmo dia/mes/ano
    if((atual_vetor_data_hora[0] == ultimo_vetor_data_hora[0]) && (atual_vetor_data_hora[1] == ultimo_vetor_data_hora[1]) && (atual_vetor_data_hora[2] == ultimo_vetor_data_hora[2])){
        //transforma horas,min, seg em segundos e subtrai
        result1 = ((ultimo_vetor_data_hora[3] * 3600) + (ultimo_vetor_data_hora[4] * 60) + ultimo_vetor_data_hora[5]);
        result2 = ((atual_vetor_data_hora[3] * 3600) + (atual_vetor_data_hora[4] * 60) + atual_vetor_data_hora[5]);

        if(result1 > result2){
            result = result1 - result2;
        }
        else if(result1 < result2){
            result = result2 - result1;
        }
        //não passou nenhum segundo, garantir que result não vai receber valores loucos
        else{
            result = 0;
        }

        //if passou 1 hora
        if (result > 150){
            return TRUE;
        }
        return FALSE;
    }
    // caso não seja o mesmo dia
    else{
        //transforma horas,min, seg em segundos e subtrai
        result1 = (86400 - ((ultimo_vetor_data_hora[3] * 3600) + (ultimo_vetor_data_hora[4] * 60) + ultimo_vetor_data_hora[5]));
        result2 = ((atual_vetor_data_hora[3] * 3600) + (atual_vetor_data_hora[4] * 60) + atual_vetor_data_hora[5]);
        //if passou 1 min
        if(result1 > result2){
            result = result1 - result2;
        }
        else if(result1 < result2){
            result = result2 - result1;
        }
        //não passou nenhum segundo, garantir que result não vai receber valores loucos
        else{
            result = 0;
        }

        //if passou 1 hora
        if (result > 150){
            return TRUE;
        }
        return FALSE;
    }

}

////////////////////////////////////////////////// Transforma dois char em inteiro /////////////////////////////////////////
int charToInt(char c) {
    return c - '0';
}

//coloca todos os valores para checagem se passou 1 min em forma de dezenas de inteiros
void coloca_min_dezena(){

    //Colocando os valores das datas e horas atuais em dezenas de int
    atual_vetor_data_hora[0] = charToInt(rtc_msg[0]) * 10 + charToInt(rtc_msg[1]);
    atual_vetor_data_hora[1] = charToInt(rtc_msg[3]) * 10 + charToInt(rtc_msg[4]);
    atual_vetor_data_hora[2] = charToInt(rtc_msg[6]) * 10 + charToInt(rtc_msg[7]);
    atual_vetor_data_hora[3] = charToInt(rtc_msg[9]) * 10 + charToInt(rtc_msg[10]);
    atual_vetor_data_hora[4] = charToInt(rtc_msg[12]) * 10 + charToInt(rtc_msg[13]);
    atual_vetor_data_hora[5] = charToInt(rtc_msg[15]) * 10 + charToInt(rtc_msg[16]);

    //Colocando os valores das datas e horas antigas (da ultima contagem, se passou o tempo desejado) em dezenas de int
    ultimo_vetor_data_hora[0] = charToInt(rtc_ultima_data_hora_1_min[0]) * 10 + charToInt(rtc_ultima_data_hora_1_min[1]);
    ultimo_vetor_data_hora[1] = charToInt(rtc_ultima_data_hora_1_min[3]) * 10 + charToInt(rtc_ultima_data_hora_1_min[4]);
    ultimo_vetor_data_hora[2] = charToInt(rtc_ultima_data_hora_1_min[6]) * 10 + charToInt(rtc_ultima_data_hora_1_min[7]);
    ultimo_vetor_data_hora[3] = charToInt(rtc_ultima_data_hora_1_min[9]) * 10 + charToInt(rtc_ultima_data_hora_1_min[10]);
    ultimo_vetor_data_hora[4] = charToInt(rtc_ultima_data_hora_1_min[12]) * 10 + charToInt(rtc_ultima_data_hora_1_min[13]);
    ultimo_vetor_data_hora[5] = charToInt(rtc_ultima_data_hora_1_min[15]) * 10 + charToInt(rtc_ultima_data_hora_1_min[16]);
}

//coloca todos os valores para checagem se passou 1 hora em forma de dezenas de inteiros
void coloca_hora_dezena(){

    //Colocando os valores das datas e horas atuais em dezenas de int
    atual_vetor_data_hora[0] = charToInt(rtc_msg[0]) * 10 + charToInt(rtc_msg[1]);
    atual_vetor_data_hora[1] = charToInt(rtc_msg[3]) * 10 + charToInt(rtc_msg[4]);
    atual_vetor_data_hora[2] = charToInt(rtc_msg[6]) * 10 + charToInt(rtc_msg[7]);
    atual_vetor_data_hora[3] = charToInt(rtc_msg[9]) * 10 + charToInt(rtc_msg[10]);
    atual_vetor_data_hora[4] = charToInt(rtc_msg[12]) * 10 + charToInt(rtc_msg[13]);
    atual_vetor_data_hora[5] = charToInt(rtc_msg[15]) * 10 + charToInt(rtc_msg[16]);

    //Colocando os valores das datas e horas antigas (da ultima contagem, se passou o tempo desejado) em dezenas de int
    ultimo_vetor_data_hora[0] = charToInt(rtc_ultima_data_hora_1_hora[0]) * 10 + charToInt(rtc_ultima_data_hora_1_hora[1]);
    ultimo_vetor_data_hora[1] = charToInt(rtc_ultima_data_hora_1_hora[3]) * 10 + charToInt(rtc_ultima_data_hora_1_hora[4]);
    ultimo_vetor_data_hora[2] = charToInt(rtc_ultima_data_hora_1_hora[6]) * 10 + charToInt(rtc_ultima_data_hora_1_hora[7]);
    ultimo_vetor_data_hora[3] = charToInt(rtc_ultima_data_hora_1_hora[9]) * 10 + charToInt(rtc_ultima_data_hora_1_hora[10]);
    ultimo_vetor_data_hora[4] = charToInt(rtc_ultima_data_hora_1_hora[12]) * 10 + charToInt(rtc_ultima_data_hora_1_hora[13]);
    ultimo_vetor_data_hora[5] = charToInt(rtc_ultima_data_hora_1_hora[15]) * 10 + charToInt(rtc_ultima_data_hora_1_hora[16]);
}

///////////////////////////////////////////////////////////////////  GPS  //////////////////////////////////////////////////////
//Função que configura e pega os dados do gps e coloca em uma string
void gps_estado_modo(){
    gps_config();
    calculo_GPRMC();
    separa_algarismos_gps_f(gps_msg);
}

///////////////////////////////////////////////////////////////////  MPU  //////////////////////////////////////////////////////

//Usando principalmente para checar o ruido com o MPU em repouso
void mpu_estado_modo(){
    char x;
    //int maiorgx = 0, menorgx = 0, maiorgy = 0, menorgy = 0, maiorgz = 0, menorgz = 0;
    //int amaiorx = 0, amenorx = 0, amaiory = 0, amenory = 0, amaiorz = 0, amenorz = 0;
    int ax,ay,az,tp,gx,gy,gz;
    int i=0;
    char vetor[14];

    x=i2c_teste_adr(MPU_ADR);
    if (x == FALSE){
        return;
    }

    while(i<300){
        i++;
        mpu_rd_vet(ACCEL_XOUT_H, vetor, 14);    //Ler 14 regs
        ax=vetor[ 0];    ax=(ax<<8)+vetor[ 1];
        ay=vetor[ 2];    ay=(ay<<8)+vetor[ 3];
        az=vetor[ 4];    az=(az<<8)+vetor[ 5];
        tp=vetor[ 6];    tp=(tp<<8)+vetor[ 7];
        gx=vetor[ 8];    gx=(gx<<8)+vetor[ 9];
        gy=vetor[10];    gy=(gy<<8)+vetor[11];
        gz=vetor[12];    gz=(gz<<8)+vetor[13];

        ser1_str("Ax"); ser1_spc(1);
        ser1_dec16(ax); ser1_spc(1);
        ser1_str("Ay"); ser1_spc(1);
        ser1_dec16(ay); ser1_spc(1);
        ser1_str("Az"); ser1_spc(1);
        ser1_dec16(az); ser1_spc(1);
        ser1_crlf(1);
        ser1_str("Gx"); ser1_spc(1);
        ser1_dec16(gx); ser1_spc(1);
        ser1_str("Gy"); ser1_spc(1);
        ser1_dec16(gy); ser1_spc(1);
        ser1_str("Gz"); ser1_spc(1);
        ser1_dec16(gz); ser1_spc(1);
        ser1_crlf(1);
//        lcdb_hex16(1,1, ax);
//        lcdb_hex16(1,6, ay);
//        lcdb_hex16(1,11,az);
//        lcdb_hex16(2,1, gx);
//        lcdb_hex16(2,6, gy);
//        lcdb_hex16(2,11,gz);

     //Checar os menores e maiores valores do MPU em estado de repouso
        //Giroscopio
        if(gx > maiorgx || maiorgx == 0) maiorgx = gx;
        else if(gx < menorgx || menorgx ==0) menorgx = gx;

        if(gy > maiorgy || maiorgy == 0) maiorgy = gy;
        else if(gy < menorgy || menorgy ==0) menorgy = gy;

        if(gz > maiorgz || maiorgz == 0) maiorgz = gz;
        else if(gz < menorgz || menorgz ==0) menorgz = gz;

        //aceleromentro
        if(ax > amaiorx || amaiorx == 0) amaiorx = ax;
        else if(ax < amenorx || amenorx ==0) amenorx = ax;

        if(ay > amaiory || amaiory == 0) amaiory = ay;
        else if(ay < amenory || amenory ==0) amenory = ay;

        if(az > amaiorz || amaiorz == 0) amaiorz = az;
        else if(az < amenorz || amenorz ==0) amenorz = az;

        delay_10ms(100);
    }

    //colocar os maiores e menores valores em um intervalo de 1 min
    gprs_str("ax= "); gprs_dec16(amenorx); gprs_str("  "); gprs_dec16(amaiorx); gprs_str("\n\r");
    gprs_str("ay= "); gprs_dec16(amenory); gprs_str("  "); gprs_dec16(amaiory); gprs_str("\n\r");
    gprs_str("az= "); gprs_dec16(amenorz); gprs_str("  "); gprs_dec16(amaiorz); gprs_str("\n\r");

    gprs_str("gx= "); gprs_dec16(menorgx); gprs_str("  "); gprs_dec16(maiorgx); gprs_str("\n\r");
    gprs_str("gy= "); gprs_dec16(menorgy); gprs_str("  "); gprs_dec16(maiorgy); gprs_str("\n\r");
    gprs_str("gz= "); gprs_dec16(menorgz); gprs_str("  "); gprs_dec16(maiorgz); gprs_str("\n\r");

    /*
     * //valores obtidos
     * ax= +16874  +16932  == 58
     * ay= -00502  -00430  == 72
     * az= -01834  -01742  == 92
     *
     * gx= -00130  -00120  == 10
     * gy= -00293  -00281  == 12
     * gz= -00320  -00311  == 9
     */
}

//Coloca os valores do estado de repouso do mpu e defino os maiores valores que cada variavel pode obter com MPU em repouso
//sera chamada no estado dormente, onde ira definir os valores para as variaveis de aceleração e giroscopio
void values_mpu(){
    char x;
    int ax,ay,az,tp,gx,gy,gz;
    char vetor[14];

    x=i2c_teste_adr(MPU_ADR);
    if (x == FALSE){
        return;
    }

    mpu_rd_vet(ACCEL_XOUT_H, vetor, 14);    //Ler 14 regs
    ax=vetor[ 0];    ax=(ax<<8)+vetor[ 1];
    ay=vetor[ 2];    ay=(ay<<8)+vetor[ 3];
    az=vetor[ 4];    az=(az<<8)+vetor[ 5];
    tp=vetor[ 6];    tp=(tp<<8)+vetor[ 7];
    gx=vetor[ 8];    gx=(gx<<8)+vetor[ 9];
    gy=vetor[10];    gy=(gy<<8)+vetor[11];
    gz=vetor[12];    gz=(gz<<8)+vetor[13];

    /*
     * //valores obtidos de um MPU especifico, é preciso verificar esse ruido para todos os outros
     * ax= +16874  +16932  == 58
     * ay= -00502  -00430  == 72
     * az= -01834  -01742  == 92
     *
     * gx= -00130  -00120  == 10
     * gy= -00293  -00281  == 12
     * gz= -00320  -00311  == 9
     */

    //Na aceleração irei colocar valores bem maiores do que do ruido, pois qualquer movimento pequeno no objeto
    // estava dando o primeiro sinal de alerta a fim de caracterizar o furto
    amaiorx = ax + 300; amenorx = ax - 300;
    amaiory = ay + 300; amenory = ay - 300;
    amaiorz = az + 300; amenorz = az - 300;


    maiorgx = gx + 10; menorgx = gx - 10;
    maiorgy = gy + 12; menorgy = gy - 12;
    maiorgz = gz + 9; menorgz = gz - 10;
}

// checa se houve mudança do acelerometro, caracterizando furto
// Se a primeira checagem der TRUE, checa durante 5 seg se ainda obtem valores diferentes do estado de repouso, dessa maneira a função retorna
// que o dispositivo foi roubado
char acel_furto(){

    char x;
    int ax,ay,az,tp,gx,gy,gz;
    int i=0, alerta=0;
    char vetor[14];

    x=i2c_teste_adr(MPU_ADR);
    if (x == FALSE){
        return FALSE;
    }

    mpu_rd_vet(ACCEL_XOUT_H, vetor, 14);    //Ler 14 regs
    ax=vetor[ 0];    ax=(ax<<8)+vetor[ 1];
    ay=vetor[ 2];    ay=(ay<<8)+vetor[ 3];
    az=vetor[ 4];    az=(az<<8)+vetor[ 5];
    tp=vetor[ 6];    tp=(tp<<8)+vetor[ 7];
    gx=vetor[ 8];    gx=(gx<<8)+vetor[ 9];
    gy=vetor[10];    gy=(gy<<8)+vetor[11];
    gz=vetor[12];    gz=(gz<<8)+vetor[13];

    //Checa se houve aleração por parte do acelerometro
    if(ax < amenorx || ax > amaiorx) alerta++;
    else if(ay < amenory || ay > amaiory) alerta++;
    else if(az < amenorz || az > amaiorz) alerta++;
    else alerta = 0;

    if(alerta == 0){
        return FALSE;
    }
    //recebeu o primeiro sinal que está com valores diferentes do repouso
    //começa a checagem por 5 segundos
    //a cada segundo checa, Caso pare de acusar falha, retorna false, caso contrario checa até encerrar 5 segundos, se for verdadeiro retorna TRUE
    //
    if(alerta != 0){
        gprs_str("alerta\n\r");
        while(i<5){
            mpu_rd_vet(ACCEL_XOUT_H, vetor, 14);    //Ler 14 regs
            ax=vetor[ 0];    ax=(ax<<8)+vetor[ 1];
            ay=vetor[ 2];    ay=(ay<<8)+vetor[ 3];
            az=vetor[ 4];    az=(az<<8)+vetor[ 5];
            tp=vetor[ 6];    tp=(tp<<8)+vetor[ 7];
            gx=vetor[ 8];    gx=(gx<<8)+vetor[ 9];
            gy=vetor[10];    gy=(gy<<8)+vetor[11];
            gz=vetor[12];    gz=(gz<<8)+vetor[13];

            //Checa se houve aleração por parte do gisroscopio
            if(ax < amenorx || gx > amaiorx) alerta++;
            else if(ay < amenory || ay > amaiory) alerta++;
            else if(az < amenorz || az > amaiorz) alerta++;
            else alerta = 0;

            if(alerta == 0){             //voltou para o estado normal
                return FALSE;
            }
            else if(alerta == 4){       //foi furtado
                return TRUE;
            }
            i++;
            delay_10ms(200); //1 seg
        }
    }
}

// checa se houve mudança do giroscopio, caracterizando furto
// Se a primeira checagem der TRUE, checa durante 5 seg se ainda obtem valores diferentes do estado de repouso, dessa maneira a função retorna
// que o dispositivo foi roubado
char giro_furto(){
    char x;
    int ax,ay,az,tp,gx,gy,gz;
    int i=0, alerta=0;
    char vetor[14];

    x=i2c_teste_adr(MPU_ADR);
    if (x == FALSE){
        return FALSE;
    }

    mpu_rd_vet(ACCEL_XOUT_H, vetor, 14);    //Ler 14 regs
    ax=vetor[ 0];    ax=(ax<<8)+vetor[ 1];
    ay=vetor[ 2];    ay=(ay<<8)+vetor[ 3];
    az=vetor[ 4];    az=(az<<8)+vetor[ 5];
    tp=vetor[ 6];    tp=(tp<<8)+vetor[ 7];
    gx=vetor[ 8];    gx=(gx<<8)+vetor[ 9];
    gy=vetor[10];    gy=(gy<<8)+vetor[11];
    gz=vetor[12];    gz=(gz<<8)+vetor[13];

    //Checa se houve aleração por parte do gisroscopio
    if(gx < menorgx || gx > maiorgx) alerta++;
    else if(gy < menorgy || gy > maiorgy) alerta++;
    else if(gz < menorgz || gz > maiorgz) alerta++;
    else alerta = 0;

    if(alerta == 0){
        return FALSE;
    }
    //recebeu o primeiro sinal que está com valores diferentes do repouso
    //começa a checagem por 5 segundos
    //a cada segundo checa, Caso pare de acusar falha, retorna false, caso contrario checa até encerrar 5 segundos, se for verdadeiro retorna TRUE
    //
    if(alerta != 0){
        gprs_str("alerta\n\r");
        while(i<5){
            mpu_rd_vet(ACCEL_XOUT_H, vetor, 14);    //Ler 14 regs
            ax=vetor[ 0];    ax=(ax<<8)+vetor[ 1];
            ay=vetor[ 2];    ay=(ay<<8)+vetor[ 3];
            az=vetor[ 4];    az=(az<<8)+vetor[ 5];
            tp=vetor[ 6];    tp=(tp<<8)+vetor[ 7];
            gx=vetor[ 8];    gx=(gx<<8)+vetor[ 9];
            gy=vetor[10];    gy=(gy<<8)+vetor[11];
            gz=vetor[12];    gz=(gz<<8)+vetor[13];

            //Checa se houve aleração por parte do gisroscopio
            if(gx < menorgx || gx > maiorgx) alerta++;
            else if(gy < menorgy || gy > maiorgy) alerta++;
            else if(gz < menorgz || gz > maiorgz) alerta++;
            else alerta = 0;

            if(alerta == 0){             //voltou para o estado normal
                return FALSE;
            }
            else if(alerta == 4){       //foi furtado
                return TRUE;
            }
            i++;
            delay_10ms(200); //1 seg
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// ENVOLVE TODAS AS FUNÇÕES ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//coloca Modo, horario, latitude, longitude, acelerometro, giroscopio e "\n\r"
void todos_dados(){
    int i;

    //salvar rtc
    for(i=0; i< 17; i++){
        toda_msg[i] = rtc_msg[i];
    }
    toda_msg[17]=' ';

    //salvar gps
    //latitude
    for(i=18; i< 29; i++){
        toda_msg[i] = gps_msg[i-18];
    }
    if(gps_msg[0]== '+'){
        toda_msg[29] = 'N';
    }
    else if(gps_msg[0]== '-'){
        toda_msg[29] = 'S';
    }
    toda_msg[30]=' ';

    //longitude
    for(i=31; i< 42; i++){
        toda_msg[i] = gps_msg[i-20];
    }
    toda_msg[42] = ',';
    if(gps_msg[11]== '+'){
        toda_msg[43] = 'E';
    }
    else if(gps_msg[11]== '-'){
        toda_msg[43] = 'W';
    }

    //salvar acelerometro e giroscopio

    //receber caracter nulo na ultima posição
    toda_msg[44]='\0';
}
