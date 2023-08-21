/*
 * Funções referente ao Serial do MSP
*/

//Separa o comando em duas strings, pega até o ultimo "#" para a primeira string e tudo depois do ultimo "#" para a segunda string
//serivira principalmente para o e-mail
void splitMessage(const String &message, String result[]) {
    int firstPos = message.indexOf('#');
    if (firstPos != -1) {
        int secondPos = message.indexOf('#', firstPos + 1);
        if (secondPos != -1) {
            result[0] = message.substring(firstPos, secondPos + 1);
            result[1] = message.substring(secondPos + 1);
        } else {
            result[0] = message;
            result[1] = "";
        }
    } else {
        result[0] = "";
        result[1] = message;
    }
}
