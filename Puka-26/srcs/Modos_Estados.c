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


//Coloca a data e hora no vetor rtc_msg
void rtc_estado(){
    rtc_rd_vet(0, rtc_vetor, 7);
    str_data_hora(rtc_vetor,rtc_msg);
}

//Atualiza o vetor que contem a data e hora refente ao ultimo envio
// So para saber se passou 1 min e 1h
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

        //if passou 1 hora = 3600 segundos
        if (result > 3600){
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

        //if passou 1 hora = 3600 segundos
        if (result > 3600){
            return TRUE;
        }
        return FALSE;
    }

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

//armazena os valores da latitude e longitude em variaveis globais
void int_latitude_longitude(){
    gps_estado_modo();

    //latitude
    lat = (gps_msg[1] -'0') * 10000000 + (gps_msg[2]-'0') * 1000000 + (gps_msg[4]-'0') * 100000;
    lataux = ((gps_msg[5]-'0'));
    lataux = (lataux * 1000); lataux = (lataux * 10); // Não estava dando certo fazer (gps_msg[5]-'0') * 10000
    lat += lataux +  (gps_msg[6]-'0') * 1000 + (gps_msg[7]-'0') * 100 + (gps_msg[8]-'0') * 10 + (gps_msg[9]-'0');

    //longitude
    //longt = gps_msg[12] * 100000000; //Não precisa no brasil os valores variam de -73.9855° a -34.7937°
    longt = (gps_msg[13]-'0') * 10000000 + (gps_msg[14]-'0') * 1000000 +(gps_msg[16]-'0') * 100000;
    longtaux = (gps_msg[17] - '0');
    longtaux = (longtaux * 1000); longtaux = (longtaux * 10); // Não estava dando certo fazer (gps_msg[5]-'0') * 10000
    longt += longtaux + (gps_msg[18]-'0') * 1000 + (gps_msg[19]-'0') * 100 + (gps_msg[20]-'0') * 10 + (gps_msg[21]-'0');
}
//coloca os maiores e menores valores que o gps pode obter para não entrar em modo de furto
void set_values_gps(){

    /*
     * Ruido obtido
     * latitude = 132 == 200
     * longitude = 237 == 300
     *
     * Ter cuidado, pois as vezes dá mais ruido do que obtido acima.
     */
    //chama a funçao responsavel por obter os valores atualizados do gps, armazena em um inteiro
    int_latitude_longitude();

    menorlatitude = lat - 250, maiorlatitude = lat + 250;
    menorlongitude = longt - 350, maiorlongitude = longt + 350;
}

// Segue o mesmo principio do MPU
// Se os novos valores forem maiores que os valores coletado no repouso + ruido. significa que houve furto
char gps_furto(){
    int i=0, alerta=0;

    //chama a funçao responsavel por obter os valores atualizados do gps, armazena em um inteiro
    int_latitude_longitude();

    if(lat< menorlatitude || lat > maiorlatitude) alerta++;
    if(longt < menorlongitude || longt > maiorlongitude) alerta++;

    if(alerta == 0) return FALSE;

    //recebeu o primeiro alerta de furto, entao checa durante 5 segundos
    else if(alerta != 0){
        //gprs_complete_str("Alerta");
        //ser1_str("Alerta"); ser1_dec32u(lat);ser1_str("  ----- "); ser1_dec32u(longt); ser1_str("\n\r");
        while(i<5){
            //chama a funçao responsavel por obter os valores atualizados do gps, armazena em um inteiro
            int_latitude_longitude();

            if(lat< menorlatitude || lat > maiorlatitude) alerta++;
            else if(longt < menorlongitude || longt > maiorlongitude) alerta++;
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
///////////////////////////////////////////////////////////////////  MPU  //////////////////////////////////////////////////////

//Armazena os valores de ax, ay, az, gx, gy e gz
void mpu_values(){
    char vetor[14];
    mpu_rd_vet(ACCEL_XOUT_H, vetor, 14);    //Ler 14 regs

    ax=vetor[ 0];    ax=(ax<<8)+vetor[ 1];
    ay=vetor[ 2];    ay=(ay<<8)+vetor[ 3];
    az=vetor[ 4];    az=(az<<8)+vetor[ 5];
    tp=vetor[ 6];    tp=(tp<<8)+vetor[ 7];
    gx=vetor[ 8];    gx=(gx<<8)+vetor[ 9];
    gy=vetor[10];    gy=(gy<<8)+vetor[11];
    gz=vetor[12];    gz=(gz<<8)+vetor[13];
}

//Usando principalmente para checar o ruido com o MPU em repouso
void mpu_estado_modo(){
    char x;
    //int maiorgx = 0, menorgx = 0, maiorgy = 0, menorgy = 0, maiorgz = 0, menorgz = 0;
    //int amaiorx = 0, amenorx = 0, amaiory = 0, amenory = 0, amaiorz = 0, amenorz = 0;
    //int ax,ay,az,tp,gx,gy,gz;
    int i=0;


    x=i2c_teste_adr(MPU_ADR);
    if (x == FALSE){
        return;
    }

    while(i<300){
        i++;
        mpu_values();

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
//Tambem será utilizada quando for salvar na memoria e precisar pegar valores recentes. Não tera problema pois a checagem do MPU era principalmente
// para caracterizar o Furto
void repouso_values_mpu(){
    char x;

    x=i2c_teste_adr(MPU_ADR);
    if (x == FALSE){
        return;
    }

    mpu_values();

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
    int i=0, alerta=0;

    x=i2c_teste_adr(MPU_ADR);
    if (x == FALSE){
        return FALSE;
    }

    mpu_values();

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
    else if(alerta != 0){
        //gprs_str("alerta\n\r");
        while(i<5){
            mpu_values();

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
    int i=0, alerta=0;

    x=i2c_teste_adr(MPU_ADR);
    if (x == FALSE){
        return FALSE;
    }

    mpu_values();

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
    else if(alerta != 0){
        //gprs_str("alerta\n\r");
        while(i<5){
            mpu_values();

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

//pega apenas os 8 bits mais significativos do MPU e salva nas variaveis globais
//Função sera chamada quando for gravar todos os dados na memoria
void mpu_8bits(){

    char x;

    x=i2c_teste_adr(MPU_ADR);
    if (x == FALSE){
        x=i2c_teste_adr(MPU_ADR);
    }

    mpu_values();
    //transformando em valores de 8 bits com sinal
    ax = ax>>8; ay = ay>>8; az = az>>8;
    gx = gx>>8; gy = gy>>8; gz = az>>8;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// MEmoria //////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Salva na memoria, chamada por Suspeito, alerta 1 e 2
//Salva o estado atual e todos os outros dados
void salvar_memoria(){
   unsigned int i=0, j=0;
    char vetor[100];
    long wr;

    rtc_estado();
    gps_estado_modo();
    todos_dados(TRUE);

    //coloca o estado atual no vetor que sera salvo na memoria
    while(estado_puka[i] != '\0'){
        vetor[i] = estado_puka[i];
        i++;
    }

    while(toda_msg[j] != '\0'){
        vetor[i] = toda_msg[j];
        i++, j++;
    }
    vetor[i] = '\n', i++, vetor[i] = '\r', i++, vetor[i] = '\0';


    i=0, wr = wr_address_mem;
    while(vetor[i] != '\0'){
        save_data(vetor[i], wr + i);
        i++;
    }
    save_data(vetor[i], wr + i);  //salvar o caractere de '\0'
    wr_address_mem+=128;
}

//Ler qual o ultimo estado salvo e pega o wr_address_mem da ultima escrita
void check_estado_address(){
    unsigned int i;
    long address=0;
    char vetor[5]; //tamanho necessario

    //che
    while(TRUE){
        wq_rd_blk(address, vetor, 5);                     //ler apenas as primeiras posições
        if(vetor[0] < ' ' || vetor[0] > 'z' ) break;    //significa que não tem mais registro
        address += 128;
    }

    wr_address_mem = address;

    //se tiver apenas o primeiro registro, vai dá ruim. Esse IF evita da erro
    if(address >= 128) address -= 128;

    wq_rd_blk(address, vetor, 5);
    //checa qual o ultimo estado para poder voltar
    if(vetor[0]=='S' && vetor[1]=='P' && vetor[2]=='T'){
        suspeito();
    }
    else if(vetor[0]=='V' && vetor[1]=='I' && vetor[2]=='G'){
        vigilia();
    }
    else if(vetor[0]=='A' && vetor[1]=='L' && vetor[2]=='T' && vetor[3]=='1'){
        alerta_1();
    }
    else if(vetor[0]=='A' && vetor[1]=='L' && vetor[2]=='T' && vetor[3]=='2'){
        alerta_2();
    }
    else{
        dormente();
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// ENVOLVE TODAS AS FUNÇÕES ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////// Transforma char em inteiro /////////////////////////////////////////
int charToInt(char c) {
    return c - '0';
}
////////////////////////////////////////////////// Transforma inteiro em char /////////////////////////////////////////
void IntToChar(int number, char *buffer){
    int i = 0, y=0;
    int isNegative = 0;
    char auxbuff[5];

    // Verifica se o número é negativo
    if (number < 0) {
        isNegative = 1;
        number = - number; // Torna o número positivo para a conversão
    }

    // Converte cada dígito para um caractere
    do {
        auxbuff[i++] = number % 10 + '0'; // Converte o dígito para o caractere ASCII
        number /= 10;
    } while (number > 0);

    // Adiciona sinal negativo, se necessário
    if (isNegative) {
        auxbuff[i++] = '-';
    }

    i--;                            //para tirar o i =='\0' do fim
    while(i>-1){
        buffer[y++] = auxbuff[i--];
    }
    buffer[y] = '\0'; // Termina a string com o caractere nulo
}

//coloca Modo, horario, latitude, longitude
// Se vier o argumento TRUE, significa que é para colocar os valores do MPU (usada para salvar na memoria)
void todos_dados(int mpu){
    unsigned int i,y;
    char buffer[5]; //8 bits podem representar apenas 3 caracteres, contando com '\0' == 4. Porem pode ter negativo entao coloquei mais 1 posição

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
    //receber caracter nulo na ultima posição
    toda_msg[44]='\0';

    //salvar acelerometro e giroscopio
    //Precisa colocar o caractere de espaço na posição 44 e guardar os novos valores referentes ao MPU
    if(mpu == TRUE){
        toda_msg[44]=' '; i=45;

        mpu_8bits();

        //Acelerometro
        toda_msg[i++]='A'; toda_msg[i++]='='; toda_msg[i++]=' ';
        //ax
        IntToChar(ax, buffer);
        y=0;
        while(buffer[y] != '\0'){
            toda_msg[i++] = buffer[y++];
        }
        toda_msg[i++]= ' ';

        //ay
        IntToChar(ay, buffer);
        y=0;
        while(buffer[y] != '\0'){
            toda_msg[i++] = buffer[y++];
        }
        toda_msg[i++]= ' ';

        //az
        IntToChar(az, buffer);
        y=0;
        while(buffer[y] != '\0'){
            toda_msg[i++] = buffer[y++];
        }
        toda_msg[i++]= ','; toda_msg[i++]= ' ';

        //Giroscopio
        toda_msg[i++]='G'; toda_msg[i++]='='; toda_msg[i++]=' ';

        //gx
        IntToChar(gx, buffer);
        y=0;
        while(buffer[y] != '\0'){
            toda_msg[i++] = buffer[y++];
        }
        toda_msg[i++]= ' ';

        //gy
        IntToChar(gy, buffer);
        y=0;
        while(buffer[y] != '\0'){
            toda_msg[i++] = buffer[y++];
        }
        toda_msg[i++]= ' ';

        //gz
        IntToChar(gz, buffer);
        y=0;
        while(buffer[y] != '\0'){
            toda_msg[i++] = buffer[y++];
        }
        toda_msg[i]= '\0';

    }
}
