bool e_mail(String mensagem, String e_mail_to, String assunto, String coordenadas){
  // Configurações do servidor SMTP2GO
  char* server = "mail.smtp2go.com";
  int port = 465;
  
  // Configurações da conta de e-mail do servidor smtp2go
  char* email = "arapuka@myyahoo.com";
  char* email_password = "tcc_joaopedro_2023";
  
  // E-mail de envio(conforme configurado no smtp2go) e recepção(qualquer e-mail)
  char* email_from = "arapukaunb@gmail.com";
  String email_to = e_mail_to;

  // Inicializar a conexão segura com o servidor SMTP
  WiFiClientSecure client;
  client.setInsecure(); // Usar essa opção somente se o servidor SMTP não tiver certificado SSL válido

  if (!client.connect(server, port)) {
    Serial.println("Falha na conexão com o servidor SMTP!");
    return false;
  }
  Serial.println("Conectado ao servidor SMTP!");

  // Esperar a resposta do servidor SMTP
  waitForResponse(client);

  // Enviar comandos SMTP para enviar o e-mail
  client.println("EHLO 192.168.1.123");
  waitForResponse(client);

  client.println("AUTH LOGIN");
  waitForResponse(client);

  client.println(base64::encode(email));
  waitForResponse(client);

  client.println(base64::encode(email_password));
  waitForResponse(client);

  client.println("MAIL FROM:<" + String(email_from) + ">");
  waitForResponse(client);

  client.println("RCPT TO:<" + String(email_to) + ">");
  waitForResponse(client);

  client.println("DATA");
  waitForResponse(client);

  client.println("Subject: " + String(assunto));
  client.println("From: <" + String(email_from) + ">");
  client.println("To: <" + String(email_to) + ">");
  client.println("Content-Type: text/plain");
  client.println("Arapuca movel\n\r" + String(mensagem) + "\n\r" + "https://www.google.com/maps?q="+ String(coordenadas)+ "\n\r att");
  client.println(".");
  waitForResponse(client);

  client.println("QUIT");
  waitForResponse(client);

  // Fechar a conexão com o servidor SMTP
  client.stop();
  return true;
}

// Função para esperar a resposta do servidor SMTP com delays adequados
void waitForResponse(WiFiClientSecure& client) {
  while (!client.available()) {
    delay(100);
  }
  while (client.available()) {
    String response = client.readStringUntil('\n');
    Serial.println(response);
  }
}
