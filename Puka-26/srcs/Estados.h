// Puka - Estados.h


#ifndef SRCS_ESTADOS_H_
#define SRCS_ESTADOS_H_

volatile unsigned int estado_i;
volatile unsigned int estado_y;
volatile char estado_comando[24];

//salva todos os dados necessarios em uma string, sera string pronta para salvar na memoria
void todos_dados(int mpu);
volatile char toda_msg[70];
char estado_puka[10];                  /// Estado atual do Arapuka

//////////////////////////////////////////////// GLOBAIS PARA O RTC ///////////////////////////////////////////////////////
volatile char rtc_vetor[7];
volatile char rtc_msg[18];

//ultima data e hora que foi mandado a mensagem de roubo, referente a 1 min
volatile char rtc_ultima_data_hora_1_min[18];

//ultima data e hora que foi mandado a mensagem de roubo, referente a 1 hora
volatile char rtc_ultima_data_hora_1_hora[18];
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////// GLOBAIS PARA A MEMORIA /////////////////////////////////////////////////
long wr_address_mem;
char mem_vetor[20];
char mem_vt[10];
volatile int n_rd,m_rd;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////// GLOBAIS PARA O GPS /////////////////////////////////////////////////////
volatile char gps_msg[22];
volatile long int menorlatitude, maiorlatitude, menorlongitude, maiorlongitude;
volatile long int lat, lataux, longt, longtaux;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////// GLOBAIS PARA O MPU /////////////////////////////////////////////////////
//recebem a aceleração e giroscopio
volatile int ax,ay,az,tp,gx,gy,gz;
volatile int maiorgx, menorgx, maiorgy, menorgy, maiorgz, menorgz;
volatile int amaiorx, amenorx, amaiory, amenory, amaiorz, amenorz;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////// ESTADOS ///////////////////////////////////////////////////////////////
void estados_config();
void sel_estado();
void resete();
void dormente();
void vigilia();
void alerta_1();
void alerta_2();
void suspeito();
void apagar();
void ler_memoria();
void ler_n();
void ler_n_m();
void rtc_configure();
void baixo_consumo();
void code_erro();
void email();
void status();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////// MODOS_ESTADOS e variaveis /////////////////////////////////////////////////////////
//////////////////////// RTC
void rtc_estado();
char passou_1_min();
char passou_1_hora();
void atualiza_data_hora(int min, int hora);
void coloca_min_dezena();
void coloca_min_dezena();
void coloca_hora_dezena();

//ultimo data e hora salva              Data e hora atualizada
volatile int ultimo_vetor_data_hora[6], atual_vetor_data_hora[6]; //cada posição refere a cada dezena da data e hora vetor[0] = dia, vetor[1] = mes, vetor[2] = ano, vetor[3] = hora, vetor[4] = min, vetor[5] = seg

////////////////////// GPS
void gps_estado_modo();
char gps_furto();
void set_values_gps();
void int_latitude_longitude();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////// MPU
void mpu_values();
void mpu_estado_modo();
void repouso_values_mpu();
char acel_furto();
char giro_furto();
void mpu_8bits();

////////////////////// MEMORIA
void ler_memoria_estado();
void salvar_memoria();
void check_estado_address();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////// Outras funções e variaveis/////////////////////////////////////////////////////////
int charToInt(char c);
void IntToChar(int number, char *buffer);

#endif /* SRCS_ESTADOS_H_ */
