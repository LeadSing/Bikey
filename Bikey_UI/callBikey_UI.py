import sys
from PyQt5.QtWidgets import QDialog, QApplication #Importa las herramientas de la librería de PyQt5
import time #Importa la librería time
import threading #Importa la librería threading que permite establecer los threads
from smbus2 import SMBus #Importa la librería smbus2 que permite establecer la comunicación I2C
from Bikey_UI import * #Importa todos los elementos del archivo Bikey_UI
import marshal

class MyForm(QDialog): #Creación de la clase MyForm que hereda de la clase QDialog
    def __init__(self): #Creación del contruscor por defecto de la clase MyForm
        super().__init__()
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)

        '''Inicialización del lcd que muestra la hora'''

        timer = QtCore.QTimer(self)
        timer.timeout.connect(self.showlcd)
        timer.start(1000)

        self.show()

    '''Función que muestra la hora '''

    def showlcd(self):
        time = QtCore.QTime.currentTime()
        text = time.toString('hh:mm')
        self.ui.lcd_hora.display(text)

class MyThread(threading.Thread): #Creación de la clase que tiene el thread que hereda de la clase threading.Thread
    bus = SMBus(1) #Inicializa la comunicación I2C

    def __init__(self, w): #Inicia el constructor por defecto de la clase threading
        threading.Thread.__init__(self)

        '''Definición de variables dentro de la clase Mythread'''

        self.w = w
        self.addr_MCU1 = 0x08  # Dirección del primer esclavo
        self.addr_MCU2 = 0x10  # Dirección del segundo esclavo
        self.datos_MCU1 = [0, 0, 0, 0, 0, 0, 0, 0, 0]  # Lista recibida del primer esclavo
        self.datos_MCU2 = [0, 0, 0, 0, 0, 0, 0, 0, 0]  # Lista recibida del segundo esclavo

    def run(self):
        print("Starting " + self.name)
        # Formato de la trama del primer microcontrolador:
        # [Combustivel(valor), cuartos (A), baja (C), alta (E), stand(G), neutro(I), izq(K), der(M)]

        #Formato de la trama del segundo microcontrolador:
        # [Velocidad (valor), Temperatura (O), Aceite(Q), Aire(S)]

        '''Contadores controladores de los cambios de las variables recibidas'''

        contaa = 0
        contac = 0
        contae = 0
        contag = 0
        contai = 0
        contak = 0
        contam = 0
        contao = 0
        contaq = 0
        contas = 0
        tiempo = 0.1

        Leer = open('distancia.txt', 'rb') #Abre el archivo distancia.txt que contiene el valor del odómetro
        odometro = marshal.load(Leer) #Lee el valor del odómetro guardado
        Leer.close() #Cierra el archivo

        while 1:
            time.sleep(tiempo) #Espera 100 milisegundos
            self.datos_MCU1 = self.bus.read_i2c_block_data(self.addr_MCU1, 0, 8) #Pide 8 bloques de datos al primer esclavo
            self.datos_MCU2 = self.bus.read_i2c_block_data(self.addr_MCU1,0,4) #Pide 4 bloques de datos al segundo esclavo

            self.w.ui.Combustible.setValue(self.datos_MCU1[0]) #Cambia el valor del indicador de gasolina
            self.w.ui.Velocidad.display(self.datos_MCU2[0]) #Cambia el valor del indicador de la velocidad

            self.w.ui.label_16.display(odometro) #Imprime el valor del odómetro

            velocidad = self.datos_MCU2[0]
            distancia = velocidad/tiempo #Calcula la distancia recorrida
            odometro += distancia #Acumula el valor anterior del odómetro con el de la distancia

            Escribir = open('distancia.txt', 'wb')
            marshal.dump(odometro, Escribir)  #Escribe el contenido actualizado del odómetro en el archivo Escribir.txt
            Escribir.close()

            if self.datos_MCU1[1] == 65: #Cuando llega el dato esperado
                self.w.ui.Luz_Cuartos.show() #Enciende el indicador de las luces cuartas
                contaa += 1 #Aumenta el contador
            elif contaa == 2: #Cuando se vuelve a recibir la variable
                self.w.ui.Luz_Cuartos.hide() #Oculta el indicador de las luces cuartas
                contaa = 0 #Reinicia el contador

            '''El proceso se repite para todos los demás valores e indicadores'''

            if self.datos_MCU1[2] == 67:
                self.w.ui.Luz_Baja.show()
                contac += 1
            elif contac == 2:
                self.w.ui.Luz_Baja.show()
                contac = 0

            if self.datos_MCU1[3] == 69:
                self.w.ui.Luz_Alta.show()
                contae += 1
            elif contae == 2:
                self.w.ui.Luz_Alta.hide()
                contae = 0

            if self.datos_MCU1[4] == 71:
                self.w.ui.Pal_Lat.show()
                contag += 1
            elif contag == 2:
                self.w.ui.Pal_Lat.hide()
                contag = 0

            if self.datos_MCU1[5] == 73:
                self.w.ui.Ind_Neutral.show()
                contai += 1
            elif contai == 2:
                self.w.ui.Ind_Neutral.hide()
                contai = 0

            if self.datos_MCU1[6] == 75:
                self.w.ui.Dir_Izq.show()
                contak += 1
            elif contak == 2:
                self.w.ui.Dir_Izq.hide()
                contak = 0

            if self.datos_MCU1[7] == 77:
                self.w.ui.Dir_Der.show()
                contam += 1
            elif contam == 2:
                self.w.ui.Dir_Der.hide()
                contam = 0

            if self.datos_MCU2[1] == 79:
                self.w.ui.Temp_Motor.show()
                contao += 1
            elif contao == 2:
                self.w.ui.Temp_Motor.hide()
                contao = 0

            if self.datos_MCU2[2] == 81:
                self.w.ui.Bajo_Aceite.show()
                contaq += 1
            elif contaq == 2:
                self.w.ui.Bajo_Aceite.hide()
                contaq = 0

            if self.datos_MCU2[3] == 83:
                self.w.ui.Filtro_Aire.show()
                contas += 1
            elif contas == 2:
                self.w.ui.Filtro_Aire.hide()
                contas = 0

            if odometro >= 6000:
                self.w.ui.Soon_Service.show()
            elif odometro <6000:
                self.w.ui.Soon_Service.hide()

            if self.datos_MCU1[0] <= 20:
                self.w.ui.Baja_Gas.show()
            elif self.datos_MCU1[0] >20:
                self.w.ui.Baja_Gas.hide()
            
            print("Finishing " + self.name)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = MyForm()
    thread1 = MyThread(w)
    thread1.start()
    w.exec()
    sys.exit(app.exec_())