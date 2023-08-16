import PySimpleGUI as sg
import requisicoes
import time
import re

'''
############################################################################################################
#                                               CONSOLE    
# ###########################################################################################################
 Utiliza Threads
'''
#Dois consoles, mostrando a resposta do POST e do GET
def console_duo(Estado):
    layout = [
        [sg.Text('Status do Post Request:')],
        [sg.Output(size=(60, 3), key='OutputPost')],
        [sg.Text('Respostas da Requisição GET:')],
        [sg.Output(size=(60, 10), key='OutputGet')],
        [sg.Button('Fechar')]
    ]
    
    window = sg.Window(Estado, layout, finalize=True, icon='arapuka.ico')
   
    return window

def update_console(output_elem, url):
    while True:
        resposta = requisicoes.get_resposta(url)
        #Restrições para que a mensagem possa ser mostrada no console
        #Mensagem que mostra quando não foi postada nenhuma mensagem desde que o site foi iniciado
        if resposta != "Nenhuma mensagem_resposta foi postada ainda.":
            output_elem.update(value=output_elem.get() + '\n' + resposta)
        time.sleep(2)

############################################################################################################
#                                               E-MAIL   
# ###########################################################################################################   
 
#checa se o email eh valido
def is_valid_email(email):
    # Utiliza uma expressão regular para validar o formato do e-mail
    pattern = r'^[\w\.-]+@[\w\.-]+\.\w+$'
    return re.match(pattern, email)

def email_layout():
    layout = [
        [sg.Text('Digite seu e-mail:')],
        [sg.InputText(key='email')],
        [sg.Button('Enviar')],
    ]
    window = sg.Window('Envio de E-mail', layout, finalize=True, icon='arapuka.ico')
   
    return window

############################################################################################################
#                                               MEMORIA  
# ###########################################################################################################  

# Ler n primeiros Registros
def rd_n():
    choices = list(range(1, 101))
    layout = [
        [sg.Text('Escolha a quantidade de Registros:')],
        [sg.Combo(choices, default_value=1, size=(10, 1), key='combo')],
        [sg.Button('Enviar')],
    ]

    window = sg.Window('RD n', layout, finalize=True, icon='arapuka.ico')

    return window

# Ler n ate m Registros
def rd_n_m():
    choices = list(range(1, 101))
    layout = [
        [sg.Text('Escolha o intervalo de Registros:')],
        [sg.Combo(choices, default_value=1, size=(10, 1), key='combo1'), sg.Combo(choices, default_value=1, size=(10, 1), key='combo2')],
        [sg.Button('Enviar')],
    ]

    window = sg.Window('RD n_m', layout, finalize=True, icon='arapuka.ico')

    return window

#checa se o primeiro valor é menor que o segundo para a ação Rd n_m
def is_valid_RD_n_m(number1, number2):
    if(number2 > number1):
        return True
    else:
        return False
    i