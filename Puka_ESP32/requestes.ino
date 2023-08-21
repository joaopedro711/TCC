const char* url_comando = "https://arapuka.onrender.com/comando";  
const char* url_resposta = "https://arapuka.onrender.com/resposta";  

// Pega o comando na pagWeb
String get_comando(){
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
  while(httpCode < 1){
    response = http.getString();
    httpCode = http.GET();
  }

  return response;
}
// Deleta o ultimo comando no servidor, para evitar pegar mais de uma vez
void delete_post_comando(){
  // Cria o objeto HTTPClient
    HTTPClient http;

    // Envia a requisição POST
    http.begin(url_comando);
    http.addHeader("Content-Type", "text/html");
    int httpResponseCode = http.POST("");

    while(httpResponseCode < 1){
      // ReEnvia a requisição POST
      Serial.println("Reenviando Requisição POST");
      httpResponseCode = http.POST("");
    }

    // Libera os recursos do objeto HTTPClient
    http.end();
}

// Envia a resposta do Arapuka para o servidor
void post_resposta(String mensagem){
  // Cria o objeto HTTPClient
    HTTPClient http;

    // Envia a requisição POST
    http.begin(url_resposta);
    http.addHeader("Content-Type", "text/html");
    int httpResponseCode = http.POST(mensagem);

    while(httpResponseCode < 1){
      // ReEnvia a requisição POST
      Serial.println("Reenviando Requisição POST");
      httpResponseCode = http.POST(mensagem);
    }

    // Libera os recursos do objeto HTTPClient
    http.end();
}
