// Extrai a latitude e longitude da mensagem vinda do Arapuka
void extractCoordenadas(const String &input, String &latitude_arapuka, String &longitude_arapuka) {
  int latStart = input.indexOf(",S") - 10;
  int latEnd = input.indexOf(",", latStart);
  latitude_arapuka = input.substring(latStart, latEnd);

  int longStart = input.indexOf(",W") - 11;
  int longEnd = input.indexOf(",", longStart);
  longitude_arapuka = input.substring(longStart, longEnd);
}
