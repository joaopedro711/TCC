import PySimpleGUI as sg
from PIL import Image
import io
import time
import sys
import os

import eventos

# diretorio_atual = os.path.dirname(os.path.abspath(__file__))
# srcs_path = os.path.abspath(os.path.join(diretorio_atual,'.', 'srcs'))
# sys.path.insert(1,srcs_path)

# from srcs.eventos import *
# from srcs.requisicoes import *

app_title = 'Arapuca'
themes = sorted(list(sg.LOOK_AND_FEEL_TABLE.keys()))
sg.ChangeLookAndFeel('DarkBlack')
sg.SetOptions(font='any 11', auto_size_buttons=True, progress_meter_border_depth=0, border_width=1)

###########################################################################################################
def introduction_animation():
    # Carregar e exibir a imagem de introdução
    intro_image = Image.open('arapuka.png')  # Substitua pelo caminho da sua imagem
    intro_image.thumbnail((300, 300))  # Redimensionar a imagem para caber na tela
    bio = io.BytesIO()
    intro_image.save(bio, format="PNG")
    layout_intro = [
        [sg.Image(data=bio.getvalue())]
    ]
    window_intro = sg.Window('Arapuca', layout_intro, finalize=True,icon='arapuka.ico')

    # Aguardar um tempo curto para exibir a imagem de introdução
    sg.popup_quick_message('Carregando...')
    time.sleep(1)
    # Fechar a janela de introdução e abrir a janela principal
    window_intro.close()
###########################################################################################################

# gui design
def create_window():
    # main tab
    layout = [[sg.T('', size=(17, 1)), sg.Text('Arapuca', font='Any 50')],

                [sg.Text('\n\n')],
                [sg.Button('Console', size=(10, 2), pad=(10, 10))],
                [sg.Text('Estados:')],
                # botões dos estados
                [sg.Button('Dormente', size=(10, 2), pad=(10, 10)), sg.Button('Vigilia', size=(10, 2), pad=(10, 10)), sg.Button('Suspeito', size=(10, 2), pad=(10, 10)), sg.Button('Alerta 1', size=(10, 2), pad=(10, 10)), sg.Button('Alerta 2', size=(10, 2), pad=(10, 10))], 
                
                
                [sg.Text('\n\nAções:')],
                #botoes das açoes extras
                [sg.Button('Status', size=(10, 2), pad=(10, 20)), sg.Button('Rd n', size=(10, 2), pad=(10, 20)), sg.Button('Rd n_m', size=(10, 2), pad=(10, 20)), sg.Button('Apagar Memoria', size=(10, 2), pad=(10, 20)), sg.Button('E-mail', size=(10, 2), pad=(10, 20))],
                [sg.Button('Resete', size=(10, 2), pad=(10, 20)), sg.Button('Set RTC', size=(10, 2), pad=(10, 20)), sg.Button('Leds', size=(10, 2), pad=(10, 20))],   
                                
            ]



    # window
    return sg.Window(title=app_title, layout=layout, size=(600, 550), icon='arapuka.ico')

introduction_animation()
window = create_window()

while True:
    event, values = window.read()

    if event == sg.WINDOW_CLOSED:   
        break

    elif event == 'Console':
        window.hide()
        eventos.log()
        window.un_hide()

    #Eventos dos Estados
    elif event == 'Dormente':
        window.hide()
        eventos.dormente()
        window.un_hide()

    elif event == 'Vigilia':
        window.hide()
        eventos.vigilia() 
        window.un_hide()  

    elif event == 'Alerta 1':
        window.hide()
        eventos.alerta_1()
        window.un_hide()

    elif event == 'Alerta 2':
        window.hide()
        eventos.alerta_2() 
        window.un_hide()

    elif event == 'Suspeito':
        window.hide()
        eventos.suspeito()
        window.un_hide()

    #Eventos das Ações
    elif event == 'Status':
        window.hide()
        eventos.status()   
        window.un_hide() 

    elif event == 'Rd n':
        window.hide()
        eventos.rd_n()
        window.un_hide()

    elif event == 'Rd n_m':
        window.hide()
        eventos.rd_n_m()  
        window.un_hide() 

    elif event == 'Apagar Memoria':
        window.hide()
        eventos.apagar_memoria()
        window.un_hide()  

    elif event == 'E-mail':
        window.hide()
        eventos.email()
        window.un_hide()

    elif event == 'Resete':
        window.hide()
        eventos.resete()
        window.un_hide()

    elif event == 'Set RTC':
        window.hide()
        eventos.rtc()
        window.un_hide()   

    elif event == 'Leds':
        window.hide()
        eventos.leds()
        window.un_hide()  


if eventos.update_thread is not None and eventos.update_thread.is_alive():
    eventos.update_thread.join()

window.close()   
sys.exit()  # Encerra o processo Python
