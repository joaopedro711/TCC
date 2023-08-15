import PySimpleGUI as sg
import requisicoes

# Funções para cada botão dos Estados
def dormente():
    #envia a requisição POST
    status_code = requisicoes.post_comando("#DMT#")
    new_window = console()
    # Verifica se a requisição foi bem-sucedida (código 200)
    if status_code == 200:
        print('Estado Dormente enviado!', file=new_window['-OUTPUT-'])
    else:
        print('Ocorreu um erro ao enviar o comando de Estado Dormente', file=new_window['-OUTPUT-'])
    while True:
        event, _ = new_window.read()
        if event == sg.WINDOW_CLOSED or event == 'Fechar':
            new_window.close()
            break 

        #requisição GET
        resposta = requisicoes.get_resposta()  
        if(resposta == 400):
            print('Ocorreu um erro ao obter resposta do comando', file=new_window['-OUTPUT-']) 
        else:
            print(resposta, file=new_window['-OUTPUT-'])  
            break 

def vigilia():
    sg.popup('Botão 2 pressionado')

def alerta_1():
    sg.popup('Botão 3 pressionado')

def alerta_2():
    sg.popup('Botão 4 pressionado')

def suspeito():
    sg.popup('Botão 5 pressionado')

# Funções para cada botão das Ações
def status():
    sg.popup('Botão 6 pressionado')

def rd_n():
    sg.popup('Botão 7 pressionado')

def rd_n_m():
    sg.popup('Botão 8 pressionado')

def apagar_memoria():
    sg.popup('Botão 9 pressionado')

def email():
    sg.popup('Botão 10 pressionado')

def resete():
    sg.popup('Botão 11 pressionado')

#Contem apenas textos
def console():
    layout = [
        [sg.Text('Console de Saída:')],
        [sg.Output(size=(40, 10), key='-OUTPUT-')],
        [sg.Button('Fechar')]
    ]
    
    window = sg.Window('Console', layout, finalize=True)
   
    return window