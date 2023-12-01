# Gerar executável, utiliza o Cx_Freeze
# Para rodar basta abrir o terminal e executar: python setup.py build

import sys
from cx_Freeze import setup, Executable

# Dependencies are automatically detected, but it might need fine tuning.
build_exe_options = {"packages": ["os", "selenium",], "includes": ["tkinter"], "include_files": ["arapuka.png", "arapuka.ico"]}

# GUI applications require a different base on Windows (the default is for
# a console application).
base = None
if sys.platform == "win32":
    base = "Win32GUI"

setup(
    name="Arapuca",
    version="1.0",
    description="Projeto de TCC",
    options={"build_exe": build_exe_options},
    executables=[Executable("arapuka.py", base=base, icon= 'arapuka.ico')]
)
