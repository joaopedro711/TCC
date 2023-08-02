// Puka - Modos.c

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

//10 - SW - Testar as chaves
char modo_sw  (char modo) {
    char sw1_ct, sw2_ct;    //Contar acionamentos das chaves
    sw1=sw2=FALSE;          //Cancelar acionamentos anteriores
    modo_ser1(modo);
    ser1_str(" Testar SW\n");
    lcdb_str(1,1,"SW");
    lcdb_str(2,1,"SW1=A  SW2=A");
    sw1_ct=sw2_ct=0;
    seri_config();    //Apagar fila de entrada
    ser1_str("SW1=");        ser1_dec8u(sw1_ct);
    ser1_str("  SW2=");      ser1_dec8u(sw2_ct);
    ser1_crlf(1);


    while(TRUE){
        if (seri_cmdo_x()==TRUE)    return modo;

        if (sw1_estado==ABERTA)  lcdb_char(2,5,'A');
        else                     lcdb_char(2,5,'F');
        if (sw2_estado==ABERTA)  lcdb_char(2,12,'A');
        else                     lcdb_char(2,12,'F');

        // SW1
        if (sw1==TRUE){
            sw1=FALSE;
            sw1_ct++;
            ser1_str("SW1=");
            ser1_dec8u(sw1_ct);
            ser1_crlf(1);
        }

        //SW2
        if (sw2==TRUE){
            sw2=FALSE;
            sw2_ct++;
            ser1_str("SW2=");
            ser1_dec8u(sw2_ct);
            ser1_crlf(1);
        }
    }
    return modo;
}

//11 - LEDs
// Testar Leds
// Digitar: VM, vn, Vm, VD, vd, Vd
char modo_leds  (char modo) {
    char qtd,argc[4],argv[10];
    modo_ser1(modo);
    ser1_str(" Testar LEDs\n");
    lcdb_str(1,1,"Leds");
    led_vm();
    led_vd();
    while(TRUE){
        qtd=seri_cmdo(argc,argv,10);
        if(qtd>0){
            if (argv[0]=='X' || argv[0]=='x'){
                led_vm();
                led_vd();
                return modo;
            }
            if      (str_cmp(&argv[0], "VM")) led_VM();
            else if (str_cmp(&argv[0], "vm")) led_vm();
            else if (str_cmp(&argv[0], "Vm")) led_Vm();
            else if (str_cmp(&argv[0], "VD")) led_VD();
            else if (str_cmp(&argv[0], "vd")) led_vd();
            else if (str_cmp(&argv[0], "Vd")) led_Vd();
            else                             ser1_char('?');
        }
    }
    return modo;
}


//12 - LCD
char modo_lcd  (char modo){
    char letra=0,base=0;
    char lin,col;
    modo_ser1(modo);
    while(TRUE){
        if (seri_cmdo_x()==TRUE)    return modo;
        for (lin=1; lin<3; lin++){
            for(col=1; col<17; col ++){
                letra=base+(lin-1)*16+(col-1);
                lcdb_char(lin,col,letra);
            }
        }
        delay_10ms(50);
        base++;
    }
    return modo;
}

//13 - Acelerometro
char modo_mpu  (char modo) {
    char x;
    int maiorx = 0, menorx = 0, maiory = 0, menory = 0, maiorz = 0, menorz = 0;
    int i , roubado = 0;
    char vetor[14];
    int ax,ay,az,tp,gx,gy,gz;
    modo_ser1(modo);
    ser1_str(" MPU =");
    lcdb_str(1,1,"MPU");

    x=i2c_teste_adr(MPU_ADR);
    if (x==TRUE)    ser1_str(" OK");
    else{
        ser1_str(" NOK");
        return modo;
    }
    ser1_crlf(1);
    roubado = 0;
    while(TRUE){
        if (seri_cmdo_x()==TRUE)    return modo;
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
        lcdb_hex16(1,1, ax);
        lcdb_hex16(1,6, ay);
        lcdb_hex16(1,11,az);
        lcdb_hex16(2,1, gx);
        lcdb_hex16(2,6, gy);
        lcdb_hex16(2,11,gz);

        /*if(gx > maiorx || maiorx == 0) maiorx = gx;
        else if(gx < menorx || menorx ==0) menorx = gx;

        if(gy > maiory || maiory == 0) maiory = gy;
        else if(gy < menory || menory ==0) menory = gy;

        if(gz > maiorz || maiorz == 0) maiorz = gz;
        else if(gz < menorz || menorz ==0) menorz = gz;

        ser1_str("Valor Atual ");
        ser1_str("Maior Valor ");
        ser1_str("Menor Valor ");
        ser1_str("\n");
        ser1_dec16(gx); ser1_spc(1);
        ser1_dec16(maiorx); ser1_spc(1);
        ser1_dec16(menorx); ser1_spc(1);
        ser1_str("\n");

        ser1_dec16(gy); ser1_spc(1);
        ser1_dec16(maiory); ser1_spc(1);
        ser1_dec16(menory); ser1_spc(1);
        ser1_str("\n");

        ser1_dec16(gz); ser1_spc(1);
        ser1_dec16(maiorz); ser1_spc(1);
        ser1_dec16(menorz); ser1_spc(1);
        ser1_str("\n");*/
        //Variáveis: X- 136 , Y- 88 , Z - 17
        delay_10ms(200);
    }
    return modo;
}

//14 - RTC - Testar o RTC
char modo_rtc  (char modo) {
    char qtd,argc[4], argv[15]; //Receber comandos
    char vetor[7];
    char msg[18];
    int i, j;
    modo_ser1(modo);
    ser1_str(" Testar RTC\n");
    lcdb_str(1,1,"RTC");
    //wq_erase_4k(0);
    while(TRUE){
        qtd=seri_cmdo(argc,argv,15);  //Esperar comando
        if (qtd!=0){
            ser1_str("Chegou: ");
            ser1_str(&argv[0]);
            ser1_crlf(1);

            if (argv[0]=='x' || argv[0]=='X')   return modo;

            // Hora (hh:mm:ss)
            if (argv[2] == ':' && argv[5]==':'){
              vetor[2]=16*(argv[0]-0x30)+(argv[1]-0x30);  //Segundos
              vetor[1]=16*(argv[3]-0x30)+(argv[4]-0x30);  //Minutos
              vetor[0]=16*(argv[6]-0x30)+(argv[7]-0x30);  //Horas
              rtc_wr_vet(0,vetor,3);
            }

            //Data (dd/mm/aa)
            if (argv[2] == '/' && argv[5]=='/'){
              vetor[0]=16*(argv[0]-0x30)+(argv[1]-0x30);  //Dia
              vetor[1]=16*(argv[3]-0x30)+(argv[4]-0x30);  //Mï¿½s
              vetor[2]=16*(argv[6]-0x30)+(argv[7]-0x30);  //Ano
              rtc_wr_vet(4,vetor,3);
            }

        }
        rtc_rd_vet(0, vetor, 7);
        ser1_data_hora(vetor);
        ser1_crlf(1);
        str_data_hora(vetor,msg);
       j = 0;
       //wq_erase_4k(0);
        for( i =0; (msg[i] != 0) ; i++){
          save_data(msg[i], j);
          j++;
        }
        lcdb_data(1,9,vetor);
        lcdb_hora(2,9,vetor);
        //ser1_hex8(x);
        //ser1_crlf(1);
        delay_10ms(100);
    }
    return modo;
}

//15 -WQ- Testar memo Flash
char modo_wq (char modo) {
    char qtd,argc[4], argv[15]; //Receber comandos
    char x,i,j;
    char vt[16];
    long wr_adr=0;  //Endereï¿½o para as escritas
    long rd_adr=0;  //Endereï¿½o para as leituras
    long er_adr=0;  //Endereï¿½o para apagar
    modo_ser1(modo);
    ser1_str(" Testar Flash\n");
    lcdb_str(1,1,"WQ");
    x=wq_sr1_rd();
    ser1_str("SR1 = ");
    ser1_hex8(x);

    x=wq_sr2_rd();
    ser1_str("  SR2 = ");
    ser1_hex8(x);
    ser1_crlf(1);

    w25_manuf_dev_id(vt);
    ser1_str("Manuf ID = ");
    ser1_hex8(vt[0]);
    ser1_spc(1);
    ser1_hex8(vt[1]);
    ser1_crlf(1);

    while(TRUE){
        qtd=0;
        while (qtd==0)  qtd=seri_cmdo(argc,argv,15);  //Esperar comando
        if (argv[0]=='x' || argv[0]=='X') return modo;

        //Leitura 256 bytes da Flash
        if (argv[0]=='r' || argv[0]=='R'){
            if (qtd>1){
                rd_adr=str_2_num(&argv[argc[1]]);
                rd_adr &= 0xFFFFFF0;        //Endereï¿½o mï¿½ltiplo de 16
            }
            ser1_str("RD ");
            ser1_hex32(rd_adr);
            ser1_crlf(1);

            for (i=0; i<16; i++){
                wq_rd_blk(rd_adr, vt,16);
                ser1_hex32(rd_adr);
                ser1_char(':');
                ser1_spc(1);
                ser1_linha(vt);
                rd_adr+=16;
            }
            ser1_crlf(1);
        }

        // WR - Escrita de um padrï¿½o de 256 bytes, opera em blocos de 16 bytes
        if (argv[0]=='w' || argv[0]=='W'){
            if (qtd>1){
                wr_adr=str_2_num(&argv[argc[1]]);
                wr_adr &= 0xFFFFFF0;        //Endereï¿½o mï¿½ltiplo de 16
            }
            ser1_str("WR ");
            ser1_hex32(wr_adr);
            ser1_crlf(1);
            x=0;
            for (i=0; i<16; i++){
                for (j=0; j<16; j++)
                    vt[j]=j+x;

                wq_wr_blk(wr_adr, vt,16);
                wr_adr+=16;
                x+=16;
            }
            ser1_str("OK");
            ser1_crlf(1);
        }

        // Erase - Apagar pï¿½gina de 4 KB (adr = aaaa aaaa   aaaa 0000   0000 0000)
        if (argv[0]=='e' || argv[0]=='E'){
            if (qtd>1){
                er_adr=str_2_num(&argv[argc[1]]);
                wr_adr &= 0xFFF000;        //Endereï¿½o mï¿½ltiplo de 4 KB
            }
            ser1_str("Erase 4K: ");
            ser1_hex32(er_adr);
            wq_erase_4k(er_adr);
            ser1_str(" OK");
            ser1_crlf(1);
        }
    }
    return modo;
}

//16 -GPS- Testar GPS
// Usa TA0
char modo_gps  (char modo) {
    char x;
    gps_config();
    modo_ser1(modo);
    ser1_str(" Testar GPS\n");
    lcdb_str(1,1,"GPS");
    while(TRUE){
        if (seri_cmdo_x()==TRUE){
            TA0CTL=0;   //Parar o timer, desligar a serial do GPS
            return modo;
        }
        if (gps_tira(&x)==TRUE)
                ser1_char(x);
    }
    return modo;
}
//17 gps com dados filtrados
char modo_gps_d(char modo){
    char x, y;
    char tipo, i = 0;
    char chegou = FALSE;
    int j = 0;
    char vetor[77];


    gps_config();
    modo_ser1(modo);
    ser1_str(" Filtrar GPS\n");
    lcdb_str(1,1,"GPS Filtro");
    while(TRUE){
        if (seri_cmdo_x()==TRUE){
            TA0CTL=0;   //Parar o timer, desligar a serial do GPS
            return modo;
        }

        if(gps_tira(&y) == TRUE){
            if(y == '$'){
                chegou = TRUE;
            }
            if(chegou == TRUE){
               vetor[i++] = y;
            }
            if(i == 76){
                if(vetor[0] == '$' && vetor[1] == 'G' && vetor[2] == 'P' && vetor[3] == 'G' && vetor[4] == 'G' && vetor[5] == 'A'){
                ser1_str(vetor);
                ser1_crlf(1);
                }
                i = 0;
                chegou = FALSE;
            }
        }
    }
    return modo;

}

//modo 18 configurar relï¿½gio rtc

char modo_configrtc(char modo){
    char aargc[10], aargv[20];
    char vetor[3];
    char i, aqtd;
    modo_ser1(modo);
    ser1_str(" Configurar RTC\n");
    ser1_str("Digite as horas com ':' ou a data com '/'.\n");
    lcdb_str(1,1,"Config");
    lcdb_str(2,1,"RTC");
        while((aqtd = seri_cmdo(aargc,aargv,20)) == 0){
            if (seri_cmdo_x()==TRUE)    return modo;
        }
         for(i=0; i<aqtd; i++){
          rtc_str(&aargv[aargc[i]]);
         }
         if (seri_cmdo_x()==TRUE)    return modo;
         //ver se ï¿½ hora
        if (vetor_rtc[2] == ':' && vetor_rtc[5]==':'){
             vetor[2]=16*(vetor_rtc[0]-0x30)+(vetor_rtc[1]-0x30);  //Horas
             vetor[1]=16*(vetor_rtc[3]-0x30)+(vetor_rtc[4]-0x30);  //Minutos
             vetor[0]=16*(vetor_rtc[6]-0x30)+(vetor_rtc[7]-0x30);  //Segundos
             rtc_wr_vet(0,vetor,3);
             ser1_str("Horas atualizada\n");
          }
        //ver se ï¿½ dia
        else if (vetor_rtc[2] == '/' && vetor_rtc[5]=='/'){
            vetor[0]=16*(vetor_rtc[0]-0x30)+(vetor_rtc[1]-0x30);  //Dia
            vetor[1]=16*(vetor_rtc[3]-0x30)+(vetor_rtc[4]-0x30);  //Mï¿½s
            vetor[2]=16*(vetor_rtc[6]-0x30)+(vetor_rtc[7]-0x30);  //Ano
            rtc_wr_vet(4,vetor,3);
            ser1_str("Data Atualizada\n");
         }
        rtc_rd_vet(0, vetor, 7);
        ser1_data_hora(vetor);
        ser1_crlf(1);
        lcdb_data(1,9,vetor);
        lcdb_hora(2,9,vetor);
        return modo;
}

//19 Teste come virgula GPS
char modo_gps_c(char modo){
    float latitude,longitude,m1 = 0,m2 = 0;
    int d1 = 0,d2 = 0;
    char x, y;
    char tipo, i = 0;
    char chegou = FALSE;
    int j = 0;
    char vetor[77];
    char filtro[43];


    gps_config();
    modo_ser1(modo);
    ser1_str(" Filtrar GPS\n");
    lcdb_str(1,1,"GPS Covertido");
    while(TRUE){
        d1 = 0;
        d2 = 0;
        m1 = 0;
        m2 = 0;
        if (seri_cmdo_x()==TRUE){
            TA0CTL=0;   //Parar o timer, desligar a serial do GPS
            return modo;
        }

        if(gps_tira(&y) == TRUE){
            if(y == '$'){
                chegou = TRUE;
            }
            if(chegou == TRUE){
               vetor[i++] = y;
            }
            if(i == 76){
                if(vetor[0] == '$' && vetor[1] == 'G' && vetor[2] == 'P' && vetor[3] == 'G' && vetor[4] == 'G' && vetor[5] == 'A'){
                    while(j < 43){
                        filtro[j] = vetor[j];
                        j++;
                    }
                j = 0;
                d1 = d1 + ((filtro[17] - 48) * 10);
                d1 = d1 + (filtro[18] - 48);
                d2 = d2 + ((filtro[30] - 48) * 100);
                d2 = d2 + ((filtro[31] - 48) * 10);
                d2 = d2 + (filtro[32] - 48);
                m1 = m1 + ((filtro[19] - 48) * 10);
                m1 = m1 + (filtro[20] - 48);
                m1 = m1 + ((filtro[22] - 48) / 10.);
                m1 = m1 + ((filtro[23] - 48) / 100.);
                m1 = m1 + ((filtro[24] - 48) / 1000.);
                m1 = m1 + ((filtro[25] - 48) / 10000.);
                m1 = m1 + ((filtro[26] - 48) / 100000.);
                m2 = m2 + ((filtro[33] - 48) * 10);
                m2 = m2 + (filtro[34] - 48);
                m2 = m2 + ((filtro[36] - 48) / 10.);
                m2 = m2 + ((filtro[37] - 48) / 100.);
                m2 = m2 + ((filtro[38] - 48) / 1000.);
                m2 = m2 + ((filtro[39] - 48) / 10000.);
                m2 = m2 + ((filtro[40] - 48) / 100000.);
                latitude  = d1 + m1/60;
                longitude  = d2 + m2/60;
                if (filtro[28] == 'S') latitude = latitude * (-1);
                if (filtro[42] == 'W') longitude = longitude * (-1);
                ser1_str(filtro);
                ser1_crlf(1);
                ser1_float(latitude, 6);
                ser1_crlf(1);
                ser1_float(longitude, 6);
                ser1_crlf(1);
                }
                i = 0;
                chegou = FALSE;
            }
        }
    }
    return modo;
}
// modo 20 - Gps filtro gprmc
char modo_gprmc (char modo) {
    char x;
    float latitude,longitude,m1 = 0,m2 = 0;
    int d1 = 0,d2 = 0;
    gps_config();
    modo_ser1(modo);
    ser1_str(" Testar GPRMC\n");
    lcdb_str(1,1,"GPRMC");
    while(TRUE){
        if (seri_cmdo_x()==TRUE){
            TA0CTL=0;   //Parar o timer, desligar a serial do GPS
            return modo;
        }
        if (gps_tira(&x)==TRUE)
                //ser1_char(x);
        if (gps_gprmc_novo==TRUE){
            gps_tranca=TRUE;        //Evitar que dados GPS sejam alterados
            gps_gprmc_novo=FALSE;
            d1 = 0;
            d2 = 0;
            m1 = 0;
            m2 = 0;
            d1 = d1 + ((gps_lat[0] - 48) * 10);
            d1 = d1 + (gps_lat[1] - 48);
            d2 = d2 + ((gps_long[0] - 48) * 100);
            d2 = d2 + ((gps_long[1] - 48) * 10);
            d2 = d2 + (gps_long[2] - 48);
            m1 = m1 + ((gps_lat[2] - 48) * 10);
            m1 = m1 + (gps_lat[3] - 48);
            m1 = m1 + ((gps_lat[5] - 48) / 10.);
            m1 = m1 + ((gps_lat[6] - 48) / 100.);
            m1 = m1 + ((gps_lat[7] - 48) / 1000.);
            m1 = m1 + ((gps_lat[8] - 48) / 10000.);
            m1 = m1 + ((gps_lat[9] - 48) / 100000.);
            m2 = m2 + ((gps_long[3] - 48) * 10);
            m2 = m2 + (gps_long[4] - 48);
            m2 = m2 + ((gps_long[6] - 48) / 10.);
            m2 = m2 + ((gps_long[7] - 48) / 100.);
            m2 = m2 + ((gps_long[8] - 48) / 1000.);
            m2 = m2 + ((gps_long[9] - 48) / 10000.);
            m2 = m2 + ((gps_long[10] - 48) / 100000.);
            latitude  = d1 + m1/60;
            longitude  = d2 + m2/60;
            if (gps_ns == 'S') latitude = latitude * (-1);
            if (gps_ew2 == 'W') longitude = longitude * (-1);

            //separa_algarismos_gps(latitude,longitude);
            ser1_str("\nGPRMC:");
            ser1_str(" Hora=");     ser1_str(gps_hora);
            ser1_str(" SAT=");      ser1_char(gps_sat);
            ser1_str(" Lat=");      ser1_float(latitude, 6);
            ser1_str(" NS=");       ser1_char(gps_ns);
            ser1_str(" Long=");     ser1_float(longitude, 6);
            ser1_str(" EW=");       ser1_char(gps_ew2);
            ser1_str(" Data=");     ser1_str(gps_data);
            ser1_crlf(1);
            gps_tranca=FALSE;        //Liberar alteraï¿½ï¿½o dados GPS
        }
    }
    return modo;
}

//separa latitude e longitude em algarismos e salva na memï¿½ria
void separa_algarismos_gps(char lat, char longi){
    int i=0 , j = 0, k;
    int lt, lt2; // algarismos inteiros da latitude
    int lg, lg2, lg3; //algarismos inteiros da longitude
    int dec; //parte decimal para envio
    float declt, declg; //partes decimais 6 casas
    char vt[22];
    long wr_adr=17;  //Endereï¿½o para comeï¿½ar as escritas

    if(lat < 0){
        vt[0] = '-';
        lat = (-1)*lat;
        i++;
    }

    if(longi <0){
        longi = (-1)*longi;
        j = 1;
    }

    lt = lat;
    lt2 = lat/10; //dezena
    lt = lt -(10*lt2); //unidade
    lg = longi;
    lg2 = lat/10;
    lg3 = lat/100; //centena
    lg2 = lg2 - (10*lg3); //dezena
    lg = lg - (10*lg2); //unidade
    vt[i] = lt2 + 0x30;
    i++;
    vt[i] = lt + 0x30;
    i++;
    vt[i] = '.';
    i++;
    declt = (lat - (lt + lt2))*10;
    for(k = 0; k < 6 ; k++){
        dec = declt; //1 digito parte decimal latitude
        vt[i] = dec + 0x30;
        i++;
        declt = (declt - dec)*10;
    }
    //fim da latitude e inï¿½cio da longitude
    if (j = 1){
        vt[i] = '-';
        i++;
        j = 0;
    }
    vt[i] = lg3 + 0x30;
    i++;
    vt[i] = lg2 + 0x30;
    i++;
    vt[i] = lg + 0x30;
    i++;

    vt[i] = '.';
    i++;
    declg = (longi - (lg + lg2 + lg3))*10;
    for(k = 0; k < 6 ; k++){
        dec = declg; //1 digito parte decimal longitude
        vt[i] = dec + 0x30;
        i++;
        declg = (declg - dec)*10;
    }
    if(i < 20){
        vt[i] = ' ';
        i++;
        vt[i] = ' ';
        i++;
    }
    else if(i < 21){
        vt[i] = ' ';
        i++;
    }
    for(k = 0; k != i; k++){
        save_data(vt[k], wr_adr + k);
    }
    /*char site[43] = "www.google.com/maps?q=";
    for(k = 22; k < 44; k++){
    site[k] = vt[k-22];
    }
    ser1_str(site);*/
}

//21 -GPRS- Testar GPRS
char modo_gprs(char modo) {
   char x;
    int j;
    int i;
    char qtd,argc[4], argv[20]; //Receber comandos
    modo_ser1(modo);
    ser1_str(" Testar GPRS\n");
    lcdb_str(1,1,"GPRS");
    while(TRUE){
        j = 0;
        while((qtd = seri_cmdo(argc,argv,20)) == 0){
            if (seri_cmdo_x()==TRUE)    return modo;
        }
        gprs_str(&argv[0]);
        ser1_str(&argv[0]);
        ser1_str("\n");
        ser0_str(&argv[0]);
//        ser0_char(vetor_gprs);
    }
    return modo;
}
//22 Salvar 1 caractï¿½re na memï¿½ria
char modo_save_char(char modo) {

    char qtd,argc[4], argv[15]; //Receber comandos
    char vt[16];
    long wr_adr=0;  //Endereï¿½o para as escritas
    long rd_adr=0;  //Endereï¿½o para as leituras
    long er_adr=0;  //Endereï¿½o para apagar
    modo_ser1(modo);
    ser1_crlf(1);

    wr_adr =  17;
    rd_adr = wr_adr;
    er_adr = wr_adr;
    wq_rd_blk(rd_adr, vt,1);
    ser1_hex8(vt[0]);
    ser1_crlf(1);

    wq_erase_4k(er_adr);
    wq_rd_blk(rd_adr, vt,1);
    ser1_hex8(vt[0]);
    ser1_crlf(1);

    while((qtd = seri_cmdo(argc,argv,20)) == 0){
        if (seri_cmdo_x()==TRUE)    return modo;
    }

    vt[0] = argv[0];
    wq_wr_blk(wr_adr, vt,1);

    wq_rd_blk(rd_adr, vt,1);
    ser1_hex8(vt[0]);
    ser1_crlf(1);

    return modo;
}
//23 Salvar mï¿½ltiplos caracteres
char modo_save(char modo) {
    char i;
    int j;
    char qtd,argc[4], argv[20]; //Receber comandos
    modo_ser1(modo);
    while(TRUE){
        while((qtd = seri_cmdo(argc,argv,20)) == 0){
            if (seri_cmdo_x()==TRUE)    return modo;
        }
        if (argv[0]=='x' || argv[0]=='X') return modo;
        j = 0;
        wq_erase_4k(0);
        for(i =0; argv[i] != 0 ; i++){
          save_data(argv[i], j);
          j++;
        }

    }
    return modo;
}
char save_data(char dt, int adr) {

    char vt[10];
    long wr_adr=0;  //Endereï¿½o para as escritas
    long rd_adr=0;  //Endereï¿½o para as leituras
    long er_adr=0;  //Endereï¿½o para apagar

    wr_adr =  adr;

    rd_adr = adr;

    vt[0] = dt;
    wq_wr_blk(wr_adr, vt,1);
    wq_rd_blk(rd_adr, vt,1);
}
//24 Envio Wifi teste AT
char send_wifi(char modo){
    char x;
     int j;
     char i;
     char qtd,argc[4], argv[20]; //Receber comandos
     modo_ser1(modo);
     ser1_str(" Testar wi-fi\n");
     lcdb_str(1,1,"Wi-Fi");
     while(TRUE){
         j = 0;
         while((qtd = seri_cmdo(argc,argv,20)) == 0){
             if (seri_cmdo_x()==TRUE)    return modo;
         }
         ser1_str(&argv[0]);
         ser1_str("\n");
         i = 'AT';
/*         tx(i,0);
         ser0_rxchar();*/
         tx(&argv[0],0);
         ser0_rxchar();
         ser0_char("A");
         ser0_rxchar();
         ser0_char("T");
         ser0_rxchar();
     }
     return modo;
}

//modo 25 salvar RTC e gprmc na memï¿½ria.
char modo_save_f(char modo){
    char msg[18], msg2[22], msg3[48];
    char teste[11], teste2[12];
    char vetor[7];
    char vt[16];
    int i, j, k;
    long wr_adr=0, rd_adr = 0;  //Endereï¿½o para comeï¿½ar as escritas e leituras.

    gps_config();

    modo_ser1(modo);
    ser1_str(" Salvar RTC + GPRMC \n");
    lcdb_str(1,1,"Salvar");

    wq_erase_4k(0);
    rtc_rd_vet(0, vetor, 7);
    str_data_hora(vetor,msg);
    calculo_GPRMC();
    separa_algarismos_gps_f(msg2);

    for(i=0; i!= 18; i++){
        msg3[i] = msg[i];
    }
    for(i=18; i!= 40; i++){
        msg3[i] = msg2[i-18];
    }
    for(i=40; i!= 48; i++){
        msg3[i] = ' ';
    }
    for(k = 0; k != i; k++){
        save_data(msg3[k], wr_adr + k);
    }

    //Ler da memoria
    for (i=0; i<16; i++){
        wq_rd_blk(rd_adr, vt,16);
        ser1_hex32(rd_adr);
        ser1_char(':');
        ser1_spc(1);
        ser1_linha(vt);
        rd_adr+=16;
    }
    j = 0;
    for(i=0; i != 22; i++){
        if(msg2[i] == ','){
            teste[i] = '\0';
            j = 1;
        }
        else if(j == 0 && msg2[i] != ',') teste[i] = msg2[i];
        else teste2[i-11] = msg2[i];
    }
    teste2[i-11] = '\0';
    ser1_crlf(1);
    lcdb_str(1,1,"Latitude=");
    delay_10ms(200);
    lcdb_apaga();
    lcdb_str(1,1,teste);
    delay_10ms(200);
    lcdb_apaga();
    lcdb_str(1,1,"Longitude=");
    delay_10ms(200);
    lcdb_apaga();
    lcdb_str(1,1,teste2);
    delay_10ms(200);

    return modo;

}

char calculo_GPRMC (void) {
    char x;
    float m1 = 0,m2 = 0;
    int d1 = 0,d2 = 0;
    gps_config();
    while(TRUE){

        if (gps_tira(&x)==TRUE)
                //ser1_char(x);
        if (gps_gprmc_novo==TRUE){
            gps_tranca=TRUE;        //Evitar que dados GPS sejam alterados
            gps_gprmc_novo=FALSE;
            d1 = 0;
            d2 = 0;
            m1 = 0;
            m2 = 0;
            d1 = d1 + ((gps_lat[0] - 48) * 10);
            d1 = d1 + (gps_lat[1] - 48);
            d2 = d2 + ((gps_long[0] - 48) * 100);
            d2 = d2 + ((gps_long[1] - 48) * 10);
            d2 = d2 + (gps_long[2] - 48);
            m1 = m1 + ((gps_lat[2] - 48) * 10);
            m1 = m1 + (gps_lat[3] - 48);
            m1 = m1 + ((gps_lat[5] - 48) / 10.);
            m1 = m1 + ((gps_lat[6] - 48) / 100.);
            m1 = m1 + ((gps_lat[7] - 48) / 1000.);
            m1 = m1 + ((gps_lat[8] - 48) / 10000.);
            m1 = m1 + ((gps_lat[9] - 48) / 100000.);
            m2 = m2 + ((gps_long[3] - 48) * 10);
            m2 = m2 + (gps_long[4] - 48);
            m2 = m2 + ((gps_long[6] - 48) / 10.);
            m2 = m2 + ((gps_long[7] - 48) / 100.);
            m2 = m2 + ((gps_long[8] - 48) / 1000.);
            m2 = m2 + ((gps_long[9] - 48) / 10000.);
            m2 = m2 + ((gps_long[10] - 48) / 100000.);
            vol_lat  = d1 + m1/60;
            vol_long  = d2 + m2/60;
            if (gps_ns == 'S') vol_lat = vol_lat * (-1);
            if (gps_ew2 == 'W') vol_long = vol_long * (-1);

            //separa_algarismos_gps(latitude,longitude);
           // ser1_str("\nGPRMC:");
           // ser1_str(" Hora=");     ser1_str(gps_hora);
            //ser1_str(" SAT=");      ser1_char(gps_sat);
            //ser1_str(" Lat=");      ser1_float(latitude, 6);
            //ser1_str(" NS=");       ser1_char(gps_ns);
            //ser1_str(" Long=");     ser1_float(longitude, 6);
            //ser1_str(" EW=");       ser1_char(gps_ew2);
            //ser1_str(" Data=");     ser1_str(gps_data);
            //ser1_crlf(1);
            gps_tranca=FALSE;        //Liberar alteraï¿½ï¿½o dados GPS
            TA0CTL=0;   //Parar o timer, desligar a serial do GPS
            return;
        }
    }
}

//separa latitude e longitude em algarismos e salva na memï¿½ria  (não salva)
void separa_algarismos_gps_f(char vt[22]){
    int a, b;
    volatile int i=0 , j = 0, k;
    volatile int lt, lt2; // algarismos inteiros da latitude
    volatile int lg, lg2, lg3; //algarismos inteiros da longitude
    volatile int dec; //parte decimal para envio
    volatile float declt, declg; //partes decimais 6 casas

    if(vol_lat < 0){
        vt[0] = '-';
        vol_lat = (-1)*vol_lat;
        i++;
    }

    if(vol_long <0){
        vol_long = (-1)*vol_long;
        j = 1;
    }
    a = vol_lat;
    b = vol_long;

    lt = vol_lat;
    lt2 = vol_lat/10; //dezena
    lt = lt -(10*lt2); //unidade
    lg = vol_long;
    lg2 = vol_long/10;
    lg3 = vol_long/100; //centena
    lg2 = lg2 - (10*lg3); //dezena
    lg = lg - (10*lg2); //unidade
    vt[i] = lt2 + 0x30;
    i++;
    vt[i] = lt + 0x30;
    i++;
    vt[i] = '.';
    i++;
    declt = (vol_lat - a)*10;
    for(k = 0; k < 6 ; k++){
        dec = declt; //1 digito parte decimal latitude
        vt[i] = dec + 0x30;
        i++;
        declt = (declt - dec)*10;
    }
    //fim da latitude e inï¿½cio da longitude
    vt[i] = ',';
    i++;
    if (j = 1){
        vt[i] = '-';
        i++;
        j = 0;
    }
    vt[i] = lg3 + 0x30;
    i++;
    vt[i] = lg2 + 0x30;
    i++;
    vt[i] = lg + 0x30;
    i++;

    vt[i] = '.';
    i++;
    declg = (vol_long - b)*10;
    for(k = 0; k < 6 ; k++){
        dec = declg; //1 digito parte decimal longitude
        vt[i] = dec + 0x30;
        i++;
        declg = (declg - dec)*10;
    }
    if(i < 20){
        vt[i] = ' ';
        i++;
        vt[i] = ' ';
        i++;
    }
    else if(i < 21){
        vt[i] = ' ';
        i++;
    }
}

//modo 26 Leitura MPU detectar furto
char modo_furto(char modo) {
    char x;
    int maiorx = 0, menorx = 0, maiory = 0, menory = 0, maiorz = 0, menorz = 0;
    int roubado, alerta = 0;
    char vetor[14];
    int ax,ay,az,tp,gx,gy,gz;

    modo_ser1(modo);
    x=i2c_teste_adr(MPU_ADR);
    if (x==TRUE)    ser1_str(" OK");
    else{
        ser1_str(" NOK");
        return modo;
    }
    ser1_crlf(1);
    roubado = 0;

    mpu_rd_vet(ACCEL_XOUT_H, vetor, 14);    //Ler 14 regs
    ax=vetor[ 0];    ax=(ax<<8)+vetor[ 1];
    ay=vetor[ 2];    ay=(ay<<8)+vetor[ 3];
    az=vetor[ 4];    az=(az<<8)+vetor[ 5];
    tp=vetor[ 6];    tp=(tp<<8)+vetor[ 7];
    gx=vetor[ 8];    gx=(gx<<8)+vetor[ 9];
    gy=vetor[10];    gy=(gy<<8)+vetor[11];
    gz=vetor[12];    gz=(gz<<8)+vetor[13];

    //Variáveis: X- 136 , Y- 88 , Z - 17
    maiorx = gx + 136;
    menorx = gx - 136;

    maiory = gy + 88;
    menory = gy - 88;

    maiorz = gz + 17;
    menorz = gz - 17;
    lcdb_apaga();
    lcdb_str(1,1,"Ativado!");

    while(TRUE){
        mpu_rd_vet(ACCEL_XOUT_H, vetor, 14);    //Ler 14 regs
        ax=vetor[ 0];    ax=(ax<<8)+vetor[ 1];
        ay=vetor[ 2];    ay=(ay<<8)+vetor[ 3];
        az=vetor[ 4];    az=(az<<8)+vetor[ 5];
        tp=vetor[ 6];    tp=(tp<<8)+vetor[ 7];
        gx=vetor[ 8];    gx=(gx<<8)+vetor[ 9];
        gy=vetor[10];    gy=(gy<<8)+vetor[11];
        gz=vetor[12];    gz=(gz<<8)+vetor[13];

        if(gx < menorx || gx > maiorx) alerta++;
        else if(gy < menory || gy > maiory) alerta++;
        else if(gz < menorz || gz > maiorz) alerta++;
        else alerta = 0;

        ser1_str("Alerta: ");
        ser1_dec16(alerta);
        ser1_crlf(1);

        ser1_str("Gx: ");
        ser1_dec16(gx);
        ser1_crlf(1);

        ser1_str("Gy: ");
        ser1_dec16(gy);
        ser1_crlf(1);

        ser1_str("Gz: ");
        ser1_dec16(gz);
        ser1_crlf(1);

        if(alerta > 4){
            roubado = 1;
            lcdb_apaga();
            ser1_str("Dispositivo furtado!");
            ser1_crlf(1);
            lcdb_str(1,1,"FURTADO");
            delay_10ms(400);
            return modo;
        }

        delay_10ms(100);
    }
}

//27 Envio e-mail
void send_email(char *email){
     char client[30] = email;
     char vt[22];
     int i = 0;
     wq_rd_blk(0, vt,17);
     ser1_str(" Enviando e-mail\n");
         ser0_char("AT+CIPMUX=1\r\n");
         delay_10ms(100);
         ser0_char("AT+CIPSERVER=1,80\r\n");
         delay_10ms(200);
         ser0_char("AT+CIPSTART=4,\"TCP\",\"smtp.sendgrid.net\",25\r\n"); //, número da conexão,protocolo, site servidor e porta
         delay_10ms(200);
         ser0_char("AT+CIPSEND=4,3\r\n");
         delay_10ms(200);
         ser0_char("HELO 192.168.1.123\r\n"); //IP no qual o dispositivo está conectado
         ser0_char("AUTH LOGIN\r\n");
         ser0_char("Bk_3QcDBR5iFKYmEtEn-FA.ooQq9AsMbq0itvHMP1g2qmCtxmccJRUqIS5BDiKpyOY=\r\n"); //email id
         ser0_char(" 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=\r\n"); //senha
         ser0_char("MAIL FROM:<ARAPUCA@gmail.com>\r\n");
         ser0_char("RCPT To:<");
         ser0_char(client);
         ser0_char(">\r\n");
         ser0_char("DATA\r\n");
         ser0_char("Subject: Dispositivo Furtado\r\n");
         ser0_char("Body Text\r\n");
         ser0_char("Nova localização do dispositivo registrado no dia ");
         for(i = 0; i <9; i++){
         ser0_char(vt[i]);
         }
         ser0_char(" no horário de ");
         for(i = 10; i <18; i++){
         ser0_char(vt[i]);
         }
         ser0_char(": ");
         wq_rd_blk(19, vt,22);
         ser0_char("https://www.google.com/maps?q=");
         ser0_char(vt);
         ser0_char(" ");
         ser0_char(".\r\n");
         delay_10ms(200);
         ser0_char("AT+CIPSEND=6\r\n");
         delay_10ms(100);
         ser0_char("QUIT\r\n");
         delay_10ms(100);
}

char modo_0   (char modo) { modo_ser1(modo); return modo;}
char modo_1   (char modo) { modo_ser1(modo); return modo;}
char modo_2   (char modo) { modo_ser1(modo); return modo;}
char modo_3   (char modo) { modo_ser1(modo); return modo;}
char modo_4   (char modo) { modo_ser1(modo); return modo;}
char modo_5   (char modo) { modo_ser1(modo); return modo;}
char modo_6   (char modo) { modo_ser1(modo); return modo;}
char modo_7   (char modo) { modo_ser1(modo); return modo;}
char modo_8   (char modo) { modo_ser1(modo); return modo;}
char modo_9   (char modo) { modo_ser1(modo); return modo;}
char modo_10  (char modo) { modo_ser1(modo); return modo;}
char modo_11  (char modo) { modo_ser1(modo); return modo;}
char modo_12  (char modo) { modo_ser1(modo); return modo;}
char modo_13  (char modo) { modo_ser1(modo); return modo;}
char modo_14  (char modo) { modo_ser1(modo); return modo;}
char modo_15  (char modo) { modo_ser1(modo); return modo;}
char modo_16  (char modo) { modo_ser1(modo); return modo;}
char modo_17  (char modo) { modo_ser1(modo); return modo;}
char modo_18  (char modo) { modo_ser1(modo); return modo;}
char modo_19  (char modo) { modo_ser1(modo); return modo;}
char modo_20  (char modo) { modo_ser1(modo); return modo;}
char modo_21  (char modo) { modo_ser1(modo); return modo;}
char modo_22  (char modo) { modo_ser1(modo); return modo;}
char modo_23  (char modo) { modo_ser1(modo); return modo;}
char modo_24  (char modo) { modo_ser1(modo); return modo;}
char modo_25  (char modo) { modo_ser1(modo); return modo;}
char modo_26  (char modo) { modo_ser1(modo); return modo;}
char modo_99  (char modo) { ser1_str("Invalido");  return modo;}

// Imprimir modo no Serial
void modo_ser1(char modo){
    ser1_str("\r\nModo ");   ser1_dec8unz(modo);
}

