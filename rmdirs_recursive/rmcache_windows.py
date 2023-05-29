import os 
import win32con, win32api

list = ['C:\Windows\Prefetch','C:\Windows\Temp']

def clear_data(locate):
    for raiz, diretorios, arquivos in os.walk(locate):
        for arquivo in arquivos:
            try:
                print(arquivo)
                win32api.SetFileAttributes(os.path.join(raiz,arquivo), win32con.FILE_ATTRIBUTE_NORMAL)
                os.remove(os.path.join(raiz,arquivo))
            except:
                print(arquivo+' Erro')

temp = os.getenv('temp')
temp = temp.replace('Roaming','\Local\Temp')
list.append(temp)

for i in list:
    clear_data(i)

exit()
