const char* url_comando = "https://arapuka.onrender.com/comando";  
const char* url_resposta = "https://arapuka.onrender.com/resposta";  

// Pega o comando na pagWeb
String get_comando(){
  int i=0;
  // Criar um objeto HTTPClient
  HTTPClient http;
  
  String response = "";
  
  // Enviar a requisição GET
  http.begin(url_comando);
  int httpCode = http.GET();

  // Verificar o código de resposta
  if (httpCode > 0) {
    // Ler o conteúdo da resposta
    response = http.getString();
  }
  while(httpCode < 1 && i < 5){
    response = http.getString();
    httpCode = http.GET();
    i++;
  }

  return response;
}
// Deleta o ultimo comando no servidor, para evitar pegar mais de uma vez
void delete_post_comando(){
  int i=0;
  // Cria o objeto HTTPClient
    HTTPClient http;

    // Envia a requisição POST
    http.begin(url_comando);
    http.addHeader("Content-Type", "text/html");
    int httpResponseCode = http.POST("");

    while(httpResponseCode < 1 && i < 5){
      // ReEnvia a requisição POST
      Serial.println("Reenviando Requisição POST");
      httpResponseCode = http.POST("");
      i++;
    }

    // Libera os recursos do objeto HTTPClient
    http.end();
}

// Envia a resposta do Arapuka para o servidor
void post_resposta(String mensagem){
  Serial.println("Inicio do Post resposta = \n\r"+ String(mensagem));
  int i = 0;
  // Cria o objeto HTTPClient
  HTTPClient http;

  // Envia a requisição POST
  http.begin(url_resposta);
  http.addHeader("Content-Type", "text/html");
  int httpResponseCode = http.POST(mensagem);

  while(httpResponseCode < 1 && i < 5){
    // ReEnvia a requisição POST
    Serial.println("Reenviando Requisição POST");
    httpResponseCode = http.POST(mensagem);
    i++;
  }
  // Libera os recursos do objeto HTTPClient
  http.end();
  delay(100);
  Serial.println("Fim do Post resposta");
}
