import PySimpleGUI as sg
from PIL import Image
import io
import time
import sys
import os

diretorio_atual = os.path.dirname(os.path.abspath(__file__))
srcs_path = os.path.abspath(os.path.join(diretorio_atual,'.', 'srcs'))
sys.path.insert(1,srcs_path)

from srcs.eventos import *

app_title = 'Arapuka'
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
    window_intro = sg.Window('Arapuka', layout_intro, finalize=True,icon='arapuka.ico')

    # Aguardar um tempo curto para exibir a imagem de introdução
    sg.popup_quick_message('Carregando...')
    time.sleep(1)
    # Fechar a janela de introdução e abrir a janela principal
    window_intro.close()
###########################################################################################################

# gui design
def create_window():
    # main tab
    layout = [[sg.T('', size=(17, 1)), sg.Text('Arapuka', font='Any 50')],

                [sg.Text('\n\n')],
                [sg.Button('Log', size=(10, 2), pad=(10, 10))],
                [sg.Text('Estados:')],
                # botões dos estados
                [sg.Button('Dormente', size=(10, 2), pad=(10, 10)), sg.Button('Vigilia', size=(10, 2), pad=(10, 10)), sg.Button('Alerta 1', size=(10, 2), pad=(10, 10)), sg.Button('Alerta 2', size=(10, 2), pad=(10, 10)), sg.Button('Suspeito', size=(10, 2), pad=(10, 10))], 
                
                
                [sg.Text('\n\nAções:')],
                #botoes das açoes extras
                [sg.Button('Status', size=(10, 2), pad=(10, 20)), sg.Button('Rd n', size=(10, 2), pad=(10, 20)), sg.Button('Rd n_m', size=(10, 2), pad=(10, 20)), sg.Button('Apagar Memoria', size=(10, 2), pad=(10, 20)), sg.Button('E-mail', size=(10, 2), pad=(10, 20))],
                [sg.Button('Resete', size=(10, 2), pad=(10, 20)), sg.Button('Set RTC', size=(10, 2), pad=(10, 20))],   
                                
            ]



    # window
    return sg.Window(title=app_title, layout=layout, size=(600, 550), icon='arapuka.ico')

introduction_animation()
window = create_window()

while True:
    event, values = window.read()

    if event == sg.WINDOW_CLOSED:   
        break

    elif event == 'Log':
        window.hide()
        log()
        window.un_hide()

    #Eventos dos Estados
    elif event == 'Dormente':
        window.hide()
        dormente()
        window.un_hide()

    elif event == 'Vigilia':
        window.hide()
        vigilia() 
        window.un_hide()  

    elif event == 'Alerta 1':
        window.hide()
        alerta_1()
        window.un_hide()

    elif event == 'Alerta 2':
        window.hide()
        alerta_2() 
        window.un_hide()

    elif event == 'Suspeito':
        window.hide()
        suspeito()
        window.un_hide()

    #Eventos das Ações
    elif event == 'Status':
        window.hide()
        status()   
        window.un_hide() 

    elif event == 'Rd n':
        window.hide()
        rd_n()
        window.un_hide()

    elif event == 'Rd n_m':
        window.hide()
        rd_n_m()  
        window.un_hide() 

    elif event == 'Apagar Memoria':
        window.hide()
        apagar_memoria()
        window.un_hide()  

    elif event == 'E-mail':
        window.hide()
        email()
        window.un_hide()

    elif event == 'Resete':
        window.hide()
        resete()
        window.un_hide()

    elif event == 'Set RTC':
        window.hide()
        rtc()
        window.un_hide()     


if update_thread is not None and update_thread.is_alive():
    update_thread.join()

window.close()   
