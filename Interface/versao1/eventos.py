import PySimpleGUI as sg
import threading
import requisicoes
import time


# Funções para cada botão dos Estados
def dormente():
    #envia a requisição POST
    post_result = requisicoes.post_comando("#DMT#")
    new_window = console()
    print(post_result, file=new_window["OutputPost"])
    # # Verifica se a requisição foi bem-sucedida (código 200)
    # if status_code == 200:
    #     print('Estado Dormente enviado!', file=new_window['OutputPost'])
    # else:
    #     print('Ocorreu um erro ao enviar o comando de Estado Dormente', file=new_window['OutputPost'])

    update_thread = threading.Thread(target=update_console, args=(new_window['OutputGet'], requisicoes.url_comando))
    update_thread.start()

    update_thread = None
    
    while True:
        event, values = new_window.read()
        if event == sg.WINDOW_CLOSED or event == 'Fechar':
            if update_thread is not None and update_thread.is_alive():
                update_thread.join()
            new_window.close()
            #arapuka.window.un_hide()
            break

        #requisição GET
        # resposta = requisicoes.get_resposta()  
        # if(resposta == 400):
        #     print('Ocorreu um erro ao obter resposta do comando', file=new_window['-OUTPUT-']) 
        # else:
        #     print(resposta, file=new_window['-OUTPUT-'])  
        #     break 
       
        # elif event == 'Iniciar GET Request':
        #     if update_thread is None or not update_thread.is_alive():
        #         get_url = "https://arapuka.onrender.com/comando"  # Substitua pela URL de GET desejada
        #         update_thread = threading.Thread(target=update_console, args=(console['-OUTPUT-'], get_url))
        #         update_thread.start()

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

'''
 #############################  CONSOLE #############################
 Utiliza Threads
'''
#Contem apenas textos
def console():
    layout = [
        [sg.Text('Status do Post Request:')],
        [sg.Output(size=(60, 5), key='OutputPost')],
        [sg.Text('Respostas da Requisição GET:')],
        [sg.Output(size=(60, 10), key='OutputGet')],
        [sg.Button('Fechar')]
    ]
    
    window = sg.Window('Console', layout, finalize=True, icon='arapuka.ico')
   
    return window

def update_console(output_elem, url):
    while True:
        resposta = requisicoes.get_resposta(url)
        output_elem.update(value=output_elem.get() + '\n' + resposta)
        time.sleep(2)