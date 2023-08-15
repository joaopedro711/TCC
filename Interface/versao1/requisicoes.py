import requests

url_status = 'https://arapuka.onrender.com/status'
url_comando = 'https://arapuka.onrender.com/comando'
url_resposta = 'https://arapuka.onrender.com/resposta'

# #posta mensagem na url /comando
# def post_comando(comando):
#     data = f"<p>{comando}</p>"
#     # Cabeçalhos da requisição
#     headers = {
#     'Content-Type': 'text/html'
#     }

#     # Faz a requisição POST
#     response = requests.post(url_comando, headers=headers, data=data)

#     # Verifica se a requisição foi bem-sucedida (código 200)
#     return response.status_code

def post_comando(comando):
    data = f"<p>{comando}</p>"
    # Cabeçalhos da requisição
    headers = {
    'Content-Type': 'text/html'
    }
    try:
        response = requests.post(url_comando, headers=headers, data=data)
        if response.status_code == 200:
            return "Mensagem enviada com sucesso."
        else:
            return f"Erro no envio da mensagem: {response.status_code}"
    except requests.exceptions.RequestException as e:
        return f"Erro no envio da mensagem: {e}"       

# #pega mensagem na url /resposta
# def get_resposta():
#     # Definir o número máximo de tentativas
#     max_attempts = 3

#     for attempt in range(1, max_attempts + 1):
#         response = requests.get(url_comando)
#         if response.status_code == 200:
#             return response.text
        
#     return response.status_code    
def get_resposta(url):
    try:
        response = requests.get(url)
        if response.status_code == 200:
            return response.text
        else:
            return f"Erro na solicitação: {response.status_code}"
    except requests.exceptions.RequestException as e:
        return f"Erro na solicitação: {e}"