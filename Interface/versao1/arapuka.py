import PySimpleGUI as sg
import eventos 
import requisicoes

app_title = 'Arapuka v1'
themes = sorted(list(sg.LOOK_AND_FEEL_TABLE.keys()))
sg.ChangeLookAndFeel('DarkBlack')
sg.SetOptions(font='any 11', auto_size_buttons=True, progress_meter_border_depth=0, border_width=1)


# gui design
def create_window():
    # main tab
    layout = [[sg.T('', size=(15, 1)), sg.Text('Arapuka', font='Any 40')],
              
                [sg.Text('\n\nEstados:')],
                # botões dos estados
                [sg.Button('Dormente', size=(10, 2), pad=(10, 10)), sg.Button('Vigilia', size=(10, 2), pad=(10, 10)), sg.Button('Alerta 1', size=(10, 2), pad=(10, 10)), sg.Button('Alerta 2', size=(10, 2), pad=(10, 10))], 
                [sg.Button('Suspeito', size=(10, 2), pad=(10, 10))],
                
                [sg.Text('\n\nAções:')],
                #botoes das açoes extras
                [sg.Button('Status', size=(10, 2), pad=(10, 20)), sg.Button('Rd n', size=(10, 2), pad=(10, 20)), sg.Button('Rd n_m', size=(10, 2), pad=(10, 20)), sg.Button('Apagar Memoria', size=(10, 2), pad=(10, 20))],
                [sg.Button('E-mail', size=(10, 2), pad=(10, 20)), sg.Button('Resete', size=(10, 2), pad=(10, 20))],                   
            ]



    # window
    return sg.Window(title=app_title, layout=layout, size=(500, 500), icon='arapuka.ico')

window = create_window()

while True:
    event, values = window.read()

    if event == sg.WINDOW_CLOSED:
        break

    #Eventos dos Estados
    elif event == 'Dormente':
        eventos.dormente()
    elif event == 'Vigilia':
        eventos.vigilia()    
    elif event == 'Alerta 1':
        eventos.alerta_1()
    elif event == 'Alerta 2':
        eventos.alerta_2() 
    elif event == 'Suspeito':
        eventos.suspeito()

    #Eventos das Ações
    elif event == 'Status':
        eventos.status()    
    elif event == 'Rd n':
        eventos.rd_n()
    elif event == 'Rd n_m':
        eventos.rd_n_m()     
    elif event == 'Apagar Memoria':
        eventos.apagar_memoria()    
    elif event == 'E-mail':
        eventos.email()
    elif event == 'Resete':
        eventos.resete()         