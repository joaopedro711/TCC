/*
 * Intermediário para a comunicação do App com o Arapuka
 *
 */
#include <WiFi.h>
#include <HTTPClient.h>


const int buttonPin = 0; // Pino GPIO0
bool buttonPressed = false;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////     Variáveis   ////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Configurações da rede WiFi à se conectar
const char* ssid = "Netvirtua_VERA";
const char* password = "40225295022";

//Variáveis Comando e Resposta
String comando = "";
String comando_result[2];   //sera usado para o email, pois divide o comando em dois
String resposta = "";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////     FUNÇÕES   //////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Funções Requestes
void post_resposta(String mensagem);
String get_comando();
void delete_post_comando();

//Funções Serial MSP
void splitMessage(const String &message, String result[]);

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
  // Verifica o estado do botão
  if (digitalRead(buttonPin) == LOW) {
    if (!buttonPressed) {
      comando = get_comando();
      splitMessage(comando, comando_result);
      delete_post_comando();
      Serial.println(comando_result[0]);
      Serial.println(comando_result[1]);
      Serial.println("--------------------");
      buttonPressed = true;
    }
  } else {
    buttonPressed = false;
  }

}