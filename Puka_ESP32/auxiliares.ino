// Extrai a latitude e longitude da mensagem vinda do Arapuka
void extractCoordenadas(const String &input, String &latitude_arapuka, String &longitude_arapuka) {
  int latStart = input.indexOf(",S") - 10;
  int latEnd = input.indexOf(",", latStart);
  latitude_arapuka = input.substring(latStart, latEnd);

  int longStart = input.indexOf(",W") - 11;
  int longEnd = input.indexOf(",", longStart);
  longitude_arapuka = input.substring(longStart, longEnd);
}

// Função que checa se houve mudança de estado, se tiver envia mensagem de whatsapp
void checa_mudanca_estado(String input){
  if(input.indexOf("DMT - ") != -1){
    WhatsAppMessages("Arapuka entrou em modo Dormente");
  }
  else if(input.indexOf("VIG - ")!= -1){
    WhatsAppMessages("Arapuka entrou em modo de Vigilia");
  }
  else if(input.indexOf("ALT1 - ")!= -1){
    WhatsAppMessages("Arapuka entrou em modo de Alerta 1");
  }
  else if(input.indexOf("ALT2 - ")!= -1){
    WhatsAppMessages("Arapuka entrou em modo de Alerta 2");
  }
  else if(input.indexOf("SPT - ")!= -1){
    WhatsAppMessages("Arapuka entrou em modo Suspeito");
  }
}
