import PySimpleGUI as sg
import threading
import requisicoes
import auxiliares


# Funções para cada botão dos Estados
def dormente():
    #envia a requisição POST
    post_result = requisicoes.post_comando("#DMT#")

    #cria console
    new_window = auxiliares.console_duo("Dormente")

    print(post_result, file=new_window["OutputPost"])   #Coloca se conseguiu enviar a mensagem ou não
    print("Estado Dormente", file=new_window["OutputPost"])
    
    #Requisição GET
    update_thread = threading.Thread(target=auxiliares.update_console, args=(new_window['OutputGet'], requisicoes.url_resposta))
    update_thread.start()
    
    while True:
        event, values = new_window.read()
        if event == sg.WINDOW_CLOSED or event == 'Fechar':
            if update_thread is not None and update_thread.is_alive():
                update_thread.join()
            new_window.close()
            break

       

def vigilia():
    #envia a requisição POST
    post_result = requisicoes.post_comando("#VIG#")

    #cria console
    new_window = auxiliares.console_duo("Vigilia")

    print(post_result, file=new_window["OutputPost"])   #Coloca se conseguiu enviar a mensagem ou não
    
    #Requisição GET
    update_thread = threading.Thread(target=auxiliares.update_console, args=(new_window['OutputGet'], requisicoes.url_resposta))
    update_thread.start()
   
    while True:
        event, values = new_window.read()
        if event == sg.WINDOW_CLOSED or event == 'Fechar':
            if update_thread is not None and update_thread.is_alive():
                update_thread.join()
            new_window.close()
            break

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
    window = auxiliares.rd_n()
    while True:
        event, values = window.read()

        if event == sg.WINDOW_CLOSED:
            break
        elif event == 'Enviar':
            window.close()
            number_n = int(values['combo'])
                            #chama a função que envia o POST 
    ###################################################################################################            
            #envia a requisição POST
            post_result = requisicoes.post_comando(f"#RD {number_n}#")

            #cria console
            new_window = auxiliares.console_duo("Leitura n Registros")

            print(post_result, file=new_window["OutputPost"])   #Coloca se conseguiu enviar a mensagem ou não
            print("Pedido de leitura de n Registros enviado!", file=new_window["OutputPost"])

            #Requisição GET
            update_thread = threading.Thread(target=auxiliares.update_console, args=(new_window['OutputGet'], requisicoes.url_resposta))
            update_thread.start()

            while True:
                event, values = new_window.read()
                if event == sg.WINDOW_CLOSED or event == 'Fechar':
                    if update_thread is not None and update_thread.is_alive():
                        update_thread.join()
                    new_window.close()
                    break
    ###################################################################################################   

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
                #envia a requisição POST
                post_result = requisicoes.post_comando(f"#RD {number_n} {number_m}#")

                #cria console
                new_window = auxiliares.console_duo("Leitura n até m Registros")

                print(post_result, file=new_window["OutputPost"])   #Coloca se conseguiu enviar a mensagem ou não
                print("Pedido de leitura de n até m Registros enviado!", file=new_window["OutputPost"])

                #Requisição GET
                update_thread = threading.Thread(target=auxiliares.update_console, args=(new_window['OutputGet'], requisicoes.url_resposta))
                update_thread.start()

                while True:
                    event, values = new_window.read()
                    if event == sg.WINDOW_CLOSED or event == 'Fechar':
                        if update_thread is not None and update_thread.is_alive():
                            update_thread.join()
                        new_window.close()
                        break
    ################################################################################################### 
            else:
                sg.popup_error('O segundo numero precisa ser maior que o primeiro.') 

    window.close()

def apagar_memoria():
    sg.popup('Botão 9 pressionado')

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
    ###################################################################################################            
                #envia a requisição POST
                post_result = requisicoes.post_comando(f"#MAIL#{email}")

                #cria console
                new_window = auxiliares.console_duo("Envio de E-mail")

                print(post_result, file=new_window["OutputPost"])   #Coloca se conseguiu enviar a mensagem ou não
                print("Pedido de E-mail enviado!", file=new_window["OutputPost"])
    
                #Requisição GET
                update_thread = threading.Thread(target=auxiliares.update_console, args=(new_window['OutputGet'], requisicoes.url_resposta))
                update_thread.start()

                while True:
                    event, values = new_window.read()
                    if event == sg.WINDOW_CLOSED or event == 'Fechar':
                        if update_thread is not None and update_thread.is_alive():
                            update_thread.join()
                        new_window.close()
                        break
    ###################################################################################################                 
            else:
                sg.popup_error('E-mail inválido. Por favor, digite novamente.')
    window.close()        

def resete():
    sg.popup('Botão 11 pressionado')

