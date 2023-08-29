/*
 * Funções referente ao Serial do MSP
*/

//Separa o comando em duas strings, pega até o ultimo "#" para a primeira string e tudo depois do ultimo "#" até o primeiro ' ' para a segunda string, o restante na 3ª string
//serivira principalmente para o e-mail
void splitMessage(const String &message, String result[]) {
    int firstPos = message.indexOf('#');
    
    if (firstPos != -1) {
        int secondPos = message.indexOf('#', firstPos + 1);
        
        if (secondPos != -1 && secondPos > firstPos) {
            result[0] = message.substring(firstPos, secondPos + 1);
            int spacePos = message.indexOf(' ', secondPos);
            
            if (spacePos != -1) {
                result[1] = message.substring(secondPos + 1, spacePos);
                result[2] = message.substring(spacePos + 1);
            } else {
                result[1] = message.substring(secondPos + 1);
                result[2] = "";
            }
        } else {
            result[0] = message;
            result[1] = "";
            result[2] = "";
        }
    } else {
        result[0] = "";
        result[1] = message;
        result[2] = "";
    }
}

//recebe o endereço da string que deverá ser enviada para o MSP430
void enviar_comando(const String &str) {
    int tamanho = str.length();
    for (int i = 0; i < tamanho; i++) {
        Serial2.print(str[i]);
        delay(2);
    }
    Serial2.print('\n');                      //só para o ultimo caractere da mensagem ser um '\n', tem que ser aspas simples para reconhecer como unico caractere
}

// Receber tipo especifico de mensagem durante alguns segundos
String receber_mensagem_t_segundos(int tempoSegundos){
  unsigned long startTime = millis();  // Armazena o tempo inicial
  String response= "";
  if (Serial2.available()) {
  while ((millis() - startTime) < (tempoSegundos * 1000)) {  // Loop durante o tempo especificado em segundos
    if (Serial2.available()) {  // Verifica se há dados disponíveis na Serial2
      response = Serial2.readStringUntil('\0');  // Lê a resposta até encontrar o caractere nulo
      if (response.charAt(0) == '#' && response.charAt(response.length() - 1) == '#') {// Verifica se o primeiro e ultimo caractere são iguais a '#'
        response += response;
      }
//      else{                         //significa que a mensagem não foi satisfeita
//        //response += "";
//        //return response;
//      }  
    }
  }  
  }
  return response;
}
//// Receber caracteres por interrupção
//void IRAM_ATTR serial_Interrupt() {
//  noInterrupts(); // Desativa interrupções
//  char character = Serial2.read();
//  if (!startRecording && character >= ' ' && character <= 'z') {
//    startRecording = true;
//    recordingStartTime = millis();
//    resposta = "";
//  } else if (startRecording) {
//    resposta += character;
//    if (millis() - recordingStartTime >= recordingDuration) {
//      startRecording = false;
//    }
//  }
//  interrupts(); // ativa interrupções
//}

// Receber tipo especifico de mensagem
String receber_mensagem(){
  String response= "";
  if (Serial2.available()) {  // Verifica se há dados disponíveis na Serial2
    response = Serial2.readStringUntil('\0');  // Lê a resposta até encontrar o caractere nulo
    if (response.charAt(0) == '#') {                                        // Verifica se o primeiro caractere é igual a '#'
      if (response.charAt(response.length() - 1) == '#') {                  // Verifica se o último caractere é igual a '#'
        return response;
      }
    }
    else{                         //significa que a mensagem não foi satisfeita
      response = "";
      return response;
    }  
  }  
}

/*
//Recebe todo tipo de mensagem
String receber_mensagem(){
  String response= "";
  if (Serial2.available()) {  // Verifica se há dados disponíveis na Serial2
    response = Serial2.readStringUntil('\0');  // Lê a resposta até encontrar o caractere nulo
    
        return response;
   }
   return response;
}
*/

// remove o primeiro e ultimo caractere
String removePrimeiroUltimoCaractere(const String& str) {
  return str.substring(1, str.length() - 1);
}
// Remove todos os caracteres de # e substitui por nada
void remove_hashtag(String &input) {
  input.replace("#", "");
}
