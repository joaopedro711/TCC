import PySimpleGUI as sg
import threading
import datetime
import queue
import requisicoes
import auxiliares

# global update_thread
# update_thread = None
update_queue = queue.Queue()
update_thread = None

def log():
    estado_com_mapa("","Console", "") #tem apenas o argumento de Estado

# Funções para cada botão dos Estados
def dormente():
    estado_duo_sem_mapa("#DMT#", "Dormente", "Estado Dormente")   

def vigilia():
    estado_duo_com_mapa("#VIG#", "Vigilia", "Estado Vigilia")    

def alerta_1():
    estado_duo_com_mapa("#ALT1#", "Alerta 1", "Estado de Alerta 1")   

def alerta_2():
    estado_duo_com_mapa("#ALT2#", "Alerta 2", "Estado de Alerta 2")   

def suspeito():
    estado_duo_com_mapa("#SPT#", "Suspeito", "Estado Suspeito") 

# Funções para cada botão das Ações
def status():
    estado_duo_com_mapa("#STAT#", "Status", "Status do Arapuca")   

def rd_n():
    window = auxiliares.rd_n()
    while True:
        event, values = window.read()

        if event == sg.WINDOW_CLOSED:
            break
        elif event == 'Enviar':
            window.close()
            number_n = int(values['combo'])
                            #chama a função que envia o POST 
            estado_duo_sem_mapa(f"#RD {number_n}#", "Leiturna de n Registros", f"Pedido de leitura de {number_n} Registros enviado!") 

    window.close()

def rd_n_m():
    window = auxiliares.rd_n_m()
    while True:
        event, values = window.read()

        if event == sg.WINDOW_CLOSED:
            break
        elif event == 'Enviar':
            
            number_n = int(values['combo1'])
            number_m = int(values['combo2'])
            #chama a função que envia o POST 
    ###################################################################################################  
            if(auxiliares.is_valid_RD_n_m(number_n, number_m)==True):    
                window.close()      
                estado_duo_sem_mapa(f"#RD {number_n} {number_m}#", "Leiturna de n até m Registros", f"Pedido de leitura do {number_n}° até o {number_m}° Registro enviado!")      
  
            else:
                sg.popup_error('O segundo numero precisa ser maior que o primeiro.',title="Números inválidos", icon='arapuka.ico') 

    window.close()

def apagar_memoria():
    estado_duo_sem_mapa("#APG#", "Apagar Memória", f"Pedido para Apagar Memória enviado!")      


def email():
    window = auxiliares.email_layout()
    while True:
        event, values = window.read()

        if event == sg.WINDOW_CLOSED:
            window.close() 
            break
        elif event == 'Enviar':
            email = values['email']
            if auxiliares.is_valid_email(email):
                window.close()
                #chama a função que envia o POST com o email e mostra o resultado no console
                estado_duo_sem_mapa(f"#MAIL#{email} {values['assunto']}", "E-mail", f"Pedido de envio de E-mail com assunto: {values['assunto']}, para o endereço {email} enviado!")      
                
            else:
                sg.popup_error('E-mail inválido. Por favor, digite novamente.', title="E-mail inválido", icon='arapuka.ico')
    window.close()        

def resete():
    estado_duo_sem_mapa(f"#RST#", "Resete", "Pedido de Resete do Arapuca enviado!")      


def rtc():
    agora = datetime.datetime.now()
    data_hora_formatada = agora.strftime("%d/%m/%y %H:%M:%S")
    #envia a requisição POST
    post_result = requisicoes.post_comando(f"#RTC {data_hora_formatada}#")
    sg.popup(f"{post_result}\nReal Time Clock: {data_hora_formatada}", title="Data e Hora", icon='arapuka.ico')

###################################################################################################
###################################################################################################
#Esta funçao fará com que acenda os leds da placa msp430
def leds():
    window = auxiliares.leds_layout()
    while True:
        event, values = window.read()

        if event == sg.WINDOW_CLOSED:
            window.close() 
            break
        elif event == 'led_verde_on':
            requisicoes.post_comando("#green on#")

        elif event == 'led_verde_off':
            requisicoes.post_comando("#green off#") 

        elif event == 'led_vermelho_on':
            requisicoes.post_comando("#red on#")
            
        elif event == 'led_vermelho_off':
            requisicoes.post_comando("#red off#")       

###################################################################################################
# ###################################################################################################    
def estado_duo_sem_mapa(comando, estado, descricao):
    ###################################################################################################     
    #envia a requisição POST
    post_result = requisicoes.post_comando(comando)

    #cria console
    new_window = auxiliares.console_duo(estado)

    print(post_result, file=new_window["OutputPost"])   #Coloca se conseguiu enviar a mensagem ou não
    print(descricao, file=new_window["OutputPost"])
    
    #Requisição GET
    global update_thread
    update_thread = threading.Thread(target=auxiliares.update_console, args=(new_window['OutputGet'], requisicoes.url_resposta, update_queue))
    update_thread.start()
    
    while True:
        event, values = new_window.read(timeout=100)
        if event == sg.WINDOW_CLOSED or event == 'Fechar':
            # if update_thread is not None and update_thread.is_alive():
            #     update_thread.join()
            #update_thread.join()               #espera terminar thread, mas enrola demais (não é boa pratica deixar sem essa ação)
            new_window.close()
            break
        try:
            resposta = update_queue.get_nowait()
            new_window['OutputGet'].update(value=new_window['OutputGet'].get() + '\n' + resposta)
        except queue.Empty:
            pass
    ###################################################################################################

def estado_duo_com_mapa(comando, estado, descricao):
    ################################################################################################### 
    #envia a requisição POST
    post_result = requisicoes.post_comando(comando)

    #cria console
    new_window = auxiliares.console_duo_map(estado)

    print(post_result, file=new_window["OutputPost"])   #Coloca se conseguiu enviar a mensagem ou não
    print(descricao, file=new_window["OutputPost"])
    
    #Requisição GET
    global update_thread
    update_thread = threading.Thread(target=auxiliares.update_console, args=(new_window['OutputGet'], requisicoes.url_resposta, update_queue))
    update_thread.start()
    
    while True:
        event, values = new_window.read(timeout=100)
        if event == sg.WINDOW_CLOSED or event == 'Fechar':
            # if update_thread is not None and update_thread.is_alive():
            #     update_thread.join()
            #update_thread.join()               #espera terminar thread, mas enrola demais (não é boa pratica deixar sem essa ação)
            new_window.close()
            break
        elif event == 'Mostrar Mapa':
            # sg.popup(f"{auxiliares.localization}")
            if auxiliares.localization != None:     #Se for diferente de vazia tenta mostrar o mapa
                latitude, longitude = auxiliares.extrair_latitude_longitude(auxiliares.localization)
                if (latitude and longitude) != False:
                    try:
                        latitude = float(latitude)
                        longitude = float(longitude)

                        thread = threading.Thread(target=auxiliares.exibir_mapa, args=(latitude, longitude))
                        thread.start()

                        # Espera a thread terminar antes de abrir a nova janela
                        thread.join()

                        try:
                            update_type, update_data = auxiliares.update_queue.get_nowait()
                            if update_type == 'show_map':
                                auxiliares.criar_janela_mapa(update_data)
                            elif update_type == 'error':
                                sg.popup_error(update_data)
                        except queue.Empty:
                            pass

                    except ValueError:
                        sg.popup_error('Valores inválidos para Latitude e Longitude.', title="Map Error", icon='arapuka.ico')
            else:            
                sg.popup_error('Ainda Não possui dados da última localização.',title="Map Error", icon='arapuka.ico')
        try:
            resposta = update_queue.get_nowait()
            new_window['OutputGet'].update(value=new_window['OutputGet'].get() + '\n' + resposta)
        except queue.Empty:
            pass
    ###################################################################################################

def estado_com_mapa(comando, estado, descricao):  
    #cria console
    new_window = auxiliares.console_get(estado)

    #Requisição GET
    global update_thread
    update_thread = threading.Thread(target=auxiliares.update_console, args=(new_window['OutputGet'], requisicoes.url_resposta, update_queue))
    update_thread.start()
    
    while True:
        event, values = new_window.read(timeout=100)
        if event == sg.WINDOW_CLOSED or event == 'Fechar':
            # if update_thread is not None and update_thread.is_alive():
            #     update_thread.join()
            #update_thread.join()               #espera terminar thread, mas enrola demais (não é boa pratica deixar sem essa ação)
            new_window.close()
            break
        elif event == 'Mostrar Mapa':
            # sg.popup(f"{auxiliares.localization}")
            if auxiliares.localization != None:     #Se for diferente de vazia tenta mostrar o mapa
                latitude, longitude = auxiliares.extrair_latitude_longitude(auxiliares.localization)
                if (latitude and longitude) != False:
                    try:
                        latitude = float(latitude)
                        longitude = float(longitude)

                        thread = threading.Thread(target=auxiliares.exibir_mapa, args=(latitude, longitude))
                        thread.start()

                        # Espera a thread terminar antes de abrir a nova janela
                        thread.join()

                        try:
                            update_type, update_data = auxiliares.update_queue.get_nowait()
                            if update_type == 'show_map':
                                auxiliares.criar_janela_mapa(update_data)
                            elif update_type == 'error':
                                sg.popup_error(update_data)
                        except queue.Empty:
                            pass

                    except ValueError:
                        sg.popup_error('Valores inválidos para Latitude e Longitude.', title="Map Error", icon='arapuka.ico')
            else:            
                sg.popup_error('Ainda Não possui dados da última localização.',title="Map Error", icon='arapuka.ico')            
        try:
            resposta = update_queue.get_nowait()
            new_window['OutputGet'].update(value=new_window['OutputGet'].get() + '\n' + resposta)
        except queue.Empty:
            pass

