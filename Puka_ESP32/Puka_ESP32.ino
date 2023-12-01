/*
 * Intermediário para a comunicação do App com o Arapuca
 *
 */
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Base64.h>
#include <UrlEncode.h>                                     

const int buttonPin = 0; // Pino GPIO0
bool buttonPressed = false;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////     Variáveis   ////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Configurações da rede WiFi à se conectar
const char* ssid = "RADNET TELECOM - 36857";
const char* password = "40225295022";

//Variáveis Comando e Resposta
String comando = "";
String comando_result[3];   //sera usado para o email, pois divide o comando em tres depois [0]= comando, [1]= email_to, [2]= Assunto
String resposta = "";
String rd_result[3];        //usada quando for o comando de RD

// Variáveis do BOT WhatsApp
String phoneNumber = "+556194142918";                                  // Meu número de celular com formato internacional
String apiKey = "9163328";   

//Variaveis para o serial com interrupção
const int interruptPin = 15;  // Pino 15 (RX2) para a interrupção
const unsigned long recordingDuration = 10000;  // Tempo de gravação em milissegundos

volatile bool startRecording = false;
unsigned long recordingStartTime;

// Latitude e longitude
String latitude_arapuka, longitude_arapuka, coordenadas;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////     FUNÇÕES   //////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Funções Requestes
void post_resposta(String mensagem);
String get_comando();
void delete_post_comando();

//Funções Serial MSP
void splitMessage(const String &message, String result[]);
void enviar_comando(const String &str);
String receber_mensagem(int tempoSegundos);
String receber_mensagem_t_segundos(int tempoSegundos);
void IRAM_ATTR serial_Interrupt();
String removePrimeiroUltimoCaractere(const String& str);
void remove_hashtag(String &input);
void rd_string(const String &message, String result[]);

// Funções e-mail
bool e_mail(String mensagem, String e_mail_to);
void waitForResponse(WiFiClientSecure& client);

// Função do whatsapp
void WhatsAppMessages(String message);

//Funções Auxiliares
void extractCoordenadas(const String &input, String &latitude_arapuka, String &longitude_arapuka);
void checa_mudanca_estado(String input);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////     SETUP/LOOP   ///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
    pinMode(buttonPin, INPUT_PULLUP); // Configura o pino do botão como entrada com pull-up interno

    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, 15, 4);  // Inicializa a comunicação serial na Serial2
   
    // Conecta-se à rede Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Conectando à rede Wi-Fi...");
    }
  
    Serial.println("Conexão Wi-Fi estabelecida!");
}

void loop() {
  resposta = "", comando = "";                                                /// Garantir que não tem resultado anterior
  
  //Checar se está conectado na internet para fazer a requisição
  if(WiFi.status() == WL_CONNECTED) {   
    //Checa se tem algum comando
    comando = get_comando();
    //5 é o comando com menor caracter
    if(comando.length() > 4 && comando != "Nenhuma mensagem_comando foi postada ainda."){
      delete_post_comando();                  // Já apaga para evitar conflito
      splitMessage(comando, comando_result);
      // envia comando para o Arapuka
      
      //Serial.println(comando_result[0]);                                        //debug
      //Serial.println(comando_result[1]);                                        //debug
      //Serial.println(comando_result[2]);                                        //debug
      // aguarda mensagem do Arapuka se não for o comando de RTC
      //se for o comando RTC
      if(comando.indexOf("#RTC") != -1){
        enviar_comando(comando_result[0]);
        post_resposta("RTC ajustado");
      }

      //se for o comando de ler da memoria, checa quantos registros deve ler e aguarda essa quantidade * 4 segundos (tempo de POST request)
      else if(comando.indexOf("#RD ") != -1){
        Serial.println("inicio RD");

          rd_string(comando, rd_result);
          enviar_comando(comando_result[0]);                             //envia o comando para o msp 
          
          //caso tenha o segundo valor de RD, ex: RD 2 4
          if(rd_result[2] != ""){
            for(int i=0; i<(rd_result[2].toInt()-rd_result[1].toInt()) + 1; i++){
              do{
                  Serial.println("---------- RD n m --------------");                                           //debugando, saber se está aqui
                  resposta = receber_mensagem();        
                }while(resposta.length() < 10);
              remove_hashtag(resposta);
              post_resposta(resposta);
            }
            Serial.println("Fim da leitura da memoria");
           post_resposta("Fim da leitura da memoria");
          }
          else{
            for(int i=0; i<rd_result[1].toInt(); i++){
              do{
                  Serial.println("---------- RD n--------------");                                           //debugando, saber se está aqui
                  resposta = receber_mensagem();        
                }while(resposta.length() < 10);
              remove_hashtag(resposta);
              post_resposta(resposta);
            }
            Serial.println("Fim da leitura da memoria");
           post_resposta("Fim da leitura da memoria");
          }
      }
      
      // caso não seja o comando de RTC, aguarda resposta do Arapuka
      
      else{
        enviar_comando(comando_result[0]);
        do{
          Serial.println("------------------------");                                           //debugando, saber se está aqui
          resposta = receber_mensagem();        
        }while(resposta.length() < 10);
        
        // Se for o comando de email posta o email
        if(comando_result[0] == "#MAIL#"){
          // Checa se tem internet para fazer o envio de email
          if(WiFi.status() == WL_CONNECTED) { 
            extractCoordenadas(resposta, latitude_arapuka, longitude_arapuka);
            coordenadas = String(latitude_arapuka) + "," + String(longitude_arapuka);
            //Serial.println(coordenadas);                                                      //debug
            remove_hashtag(resposta);
            //Serial.println(resposta);                                                      //debug
            //envia email com smtp2go
            e_mail(resposta, comando_result[1], comando_result[2], coordenadas);
            post_resposta("E-mail enviado");
          }
        }
        //Se não for o comando de Email posta toda a mensagem no servidor de resposta
        else{
          // Checa se tem internet para fazer o Post da resposta
          if(WiFi.status() == WL_CONNECTED) { 
            remove_hashtag(resposta);
            post_resposta(resposta);
            
            //Se não foi comando de Status, checa se mudou de estado
            if(comando_result[0] != "#STAT#"){
              //Checa se Chegou mudança de ESTADO
              //Envia mensagem para o Whatsapp
              checa_mudanca_estado(resposta);
            }
          }
        }
      }
    }
  }
  resposta = "";                                                /// Garantir que não tem resultado anterior
  
  //obtem resposta do Arapuka
  resposta = receber_mensagem();
  Serial.println("||||||||||||||||||||||||||||||||||||||||");  //debugando, saber se está aqui
  //Faz Post da resposta caso tenha chegado
  if(resposta.length() > 10){
    if(WiFi.status() == WL_CONNECTED) { 
      remove_hashtag(resposta);
      post_resposta(resposta);
      //Checa se Chegou mudança de ESTADO
      //Envia mensagem para o Whatsapp
      checa_mudanca_estado(resposta);
    }
  }
}
