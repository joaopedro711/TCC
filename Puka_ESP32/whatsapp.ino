//Envia mensagem para o whatsApp (precisa inserir o numero e Key nas variaveis globais da main)
void WhatsAppMessages(String message){
    int i=0;
    //Serial.println("Iniciando Post");                                         //Debug
    // Dados a serem enviados pelo método Post suportado pelo protocolo HTTP
    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
    
    HTTPClient http;                                                            // Cria um objeto HTTP
    http.begin(url);                                                            // Inicia o objeto HTTP passando a string acima
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");        // Adiciona um cabeçalho
  
    uint16_t httpResponseCode = http.POST(url);                                 // Envia o método POST HTTP de requisição e lê o resultado
    //Serial.println(url);                                                      //Debug
    while(httpResponseCode != 200 && i<5){  
        httpResponseCode = http.POST(url);
        i++;
        Serial.println("Deu ruim");                                             //Debug
    }
    
    http.end();                                                                 // Finaliza o objeto http
}
