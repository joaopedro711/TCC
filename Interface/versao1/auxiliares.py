import PySimpleGUI as sg
import requisicoes
import time
import re
import threading
import folium
from selenium import webdriver
import os
import queue

#global que salva a localização para mostrar o mapa
localization = None

# Cria uma fila de mensagens para atualizações na janela
global update_queue 
update_queue = queue.Queue()

'''
############################################################################################################
#                                               CONSOLE    
# ###########################################################################################################
 Utiliza Threads
'''
#Dois consoles sem o mapa, mostrando a resposta do POST e do GET
def console_duo(Estado):
    layout = [
        [sg.Text('Status do Post Request:')],
        [sg.Output(size=(70, 3), key='OutputPost')],
        [sg.Text('Respostas da Requisição GET:')],
        [sg.Output(size=(70, 10), key='OutputGet')],
        [sg.Button('Fechar')]
    ]
    
    window = sg.Window(Estado, layout, finalize=True, icon='arapuka.ico')
   
    return window

#Dois consoles com o mapa, mostrando a resposta do POST e do GET
def console_duo_map(Estado):
    layout = [
        [sg.Text('Status do Post Request:')],
        [sg.Output(size=(50, 3), key='OutputPost'),sg.T('', size=(4, 1)), sg.Button('Mostrar Mapa', size=(10, 2), pad=(10, 10))],
        [sg.Text('Respostas da Requisição GET:')],
        [sg.Output(size=(70, 10), key='OutputGet')],
        [sg.Button('Fechar')]
    ]
    
    window = sg.Window(Estado, layout, finalize=True, icon='arapuka.ico')
   
    return window

#único console, mostrando a resposta do GET, com opção de mostrar mapa
def console_get(Estado):
    layout = [
        [sg.Button('Mostrar Mapa', size=(10, 2), pad=(10, 10))],
        [sg.Text('Respostas da Requisição GET:')],
        [sg.Output(size=(70, 10), key='OutputGet')],
        [sg.Button('Fechar')]
    ]
    
    window = sg.Window(Estado, layout, finalize=True, icon='arapuka.ico')
   
    return window

#atualiza toda parte GET do Console e coloca janela com mapa antes de apagar a mensagem
def update_console(output_elem, url, update_queue):
    global localization
    while True:
        resposta = requisicoes.get_resposta(url)
        #Restrições para que a mensagem possa ser mostrada no console
        #Mensagem que mostra quando não foi postada nenhuma mensagem desde que o site foi iniciado
        if resposta != "Nenhuma mensagem_resposta foi postada ainda.":
            if len(resposta) > 30:
                localization = resposta
            # output_elem.update(value=output_elem.get() + '\n' + resposta)
            # #apaga a ultima resposta, garante que vai pegar apenas uma vez a resposta
            update_queue.put(resposta)
            requisicoes.delete_resposta()
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
        [sg.Text('E-mail de destino:'), sg.InputText(key='email')],
        [sg.Text('Assunto do E-mail:'), sg.InputText(key='assunto')],
        [sg.Button('Enviar')],
    ]
    window = sg.Window('Envio de E-mail', layout, finalize=True, icon='arapuka.ico')
   
    return window

############################################################################################################
#                                               MEMORIA  
# ###########################################################################################################  

# Ler n primeiros Registros
def rd_n():
    choices = list(range(1, 801))
    layout = [
        [sg.Text('Escolha a quantidade de Registros:')],
        [sg.Combo(choices, default_value=1, size=(10, 1), key='combo')],
        [sg.Button('Enviar')],
    ]

    window = sg.Window('RD n', layout, finalize=True, icon='arapuka.ico')

    return window

# Ler n ate m Registros
def rd_n_m():
    choices = list(range(1, 801))
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

#Layout para botões dos leds
def leds_layout():
    layout = [
        [sg.Text('Led Verde:'), sg.Text('  '), sg.Button('ON', button_color=('white', 'green'), key='led_verde_on'), sg.Button('OFF', button_color=('white', 'red'), key='led_verde_off')],
        [sg.Text('Led Vermelho:'), sg.Button('ON', button_color=('white', 'green'), key='led_vermelho_on'), sg.Button('OFF', button_color=('white', 'red'), key='led_vermelho_off')],
    ]
    window = sg.Window('Leds', layout, finalize=True, icon='arapuka.ico')

    return window
############################################################################################################
#                                               MAPA
# ###########################################################################################################   

#retira a latitude e longitude da mensagem completa  
def extrair_latitude_longitude(data):
    try:
        partes = data.split()
        # Significa que mudou de estado
        if(partes[0] == "DMT" or partes[0] == "VIG" or partes[0] == "ALT1" or partes[0] == "ALT2" or partes[0] == "SPT"):
            latitude_str, longitude_str = partes[4].split(','), partes[5].split(',')
            latitude = float(latitude_str[0])
            longitude = float(longitude_str[0])
            return latitude, longitude
        # Não mudou de estado
        else:    
            latitude_str, longitude_str = partes[2].split(','), partes[3].split(',')
            latitude = float(latitude_str[0])
            longitude = float(longitude_str[0])
            return latitude, longitude   
    except:
        return False, False 
    
def exibir_mapa(latitude, longitude):
    try:
        m = folium.Map(location=[latitude, longitude], zoom_start=16)
        folium.Marker([latitude, longitude]).add_to(m)

        map_filename = 'map.html'
        m.save(map_filename)

        image_filename = 'map_screenshot.png'
        capture_screenshot(map_filename, image_filename)

        os.remove(map_filename)

        update_queue.put(('show_map', image_filename))
    except Exception as e:
        update_queue.put(('error', str(e)))

def capture_screenshot(url, output_filename):
    options = webdriver.ChromeOptions()
    options.add_argument('--headless')
    driver = webdriver.Chrome(options=options)

    driver.get('file://' + os.path.abspath(url))
    driver.save_screenshot(output_filename)

    driver.quit()

def criar_janela_mapa(image_filename):
    layout_mapa = [[sg.Image(filename=image_filename)]]
    map_window = sg.Window('Mapa', layout_mapa, finalize=True, icon='arapuka.ico')

    while True:
        event, values = map_window.read()

        if event == sg.WINDOW_CLOSED:
            map_window.close()
            os.remove(image_filename)
            break