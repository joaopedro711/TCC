from flask import Flask, request

app = Flask(__name__)

@app.route("/", methods=["GET"])
def homepage():
    return '''
        <html>
        <head>
            <title>Arapuka</title>
        </head>
        <body>   
            <br/><br/><br/><br/><br/><br/><br/><br/><br/>
            <center><h1>Bem vido ao Arapuka móvel</h1></center>
            <br/><br/><br/><br/><br/><br/><br/><br/><br/>
            <center><p>Trabalho de conclusão de curso</p></center>
        </body>
        </html>
    '''

@app.route("/comando", methods=["GET", "POST"])
def comando():
    if request.method == "GET":
        if 'mensagem_comando' in app.config:                        #caso exista alguma mensagem_comando na variável de configuração
            mensagem_comando = app.config['mensagem_comando']
            return mensagem_comando
        else:
            return 'Nenhuma mensagem_comando foi postada ainda.'
    
    elif request.method == "POST":
        mensagem_comando = request.data.decode('utf-8')             # Obtém a mensagem do corpo (se for html) da requisição
        app.config['mensagem_comando'] = mensagem_comando           # Armazena a mensagem_comando na variável de configuração
        return 'Mensagem_comando postada'

@app.route("/resposta", methods=["GET", "POST"])
def resposta():
    if request.method == "GET":
        if 'mensagem_resposta' in app.config:                        #caso exista alguma mensagem_resposta na variável de configuração
            mensagem_resposta = app.config['mensagem_resposta']
            return mensagem_resposta
        else:
            return 'Nenhuma mensagem_resposta foi postada ainda.'
    
    elif request.method == "POST":
        mensagem_resposta = request.data.decode('utf-8')             # Obtém a mensagem_resposta do corpo (se for html) da requisição
        app.config['mensagem_resposta'] = mensagem_resposta          # Armazena a mensagem na variável de configuração
        return 'Mensagem_resposta postada'
    
@app.route("/status", methods=["GET", "POST"])
def status():
    if request.method == "GET":
        if 'mensagem_status' in app.config:                        #caso exista alguma mensagem_status na variável de configuração
            mensagem_status = app.config['mensagem_status']
            return mensagem_status                                 # se chegar aqui, a mensagem é "ON"
        else:
            return 'Status OFF'
    
    elif request.method == "POST":
        mensagem_status = request.data.decode('utf-8')             # Obtém a mensagem_resposta do corpo (se for html) da requisição
        app.config['mensagem_status'] = mensagem_status            # Armazena a mensagem na variável de configuração
        return 'Status ON'
    
if __name__ == "__main__":
    app.run(debug=True)