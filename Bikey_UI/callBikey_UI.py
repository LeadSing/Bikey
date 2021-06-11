import sys
from PyQt5.QtWidgets import QDialog, QApplication
import time
import threading
from smbus2 import SMBus
from Bikey_UI import *

class MyForm(QDialog):
    def __init__(self):
        super().__init__()
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)
        self.show()


class MyThread(threading.Thread):
    bus = SMBus(1)
    addr_MCU1 = 0x08
    addr_MCU2 = 0x10
    datos_MCU1 = [0, 0, 0, 0, 0, 0, 0, 0, 0]
    datos_MCU2 = [0, 0, 0, 0, 0, 0, 0, 0, 0]
    contaa = 0
    contab = 0
    contae = 0
    contag = 0
    contai = 0
    contak = 0
    contam = 0
    contao = 0
    contaq = 0
    contas = 0


    def __init__(self, w):
        threading.Thread.__init__(self)
        self.w = w
        self.addr_MCU1 = 0x08
        self.addr_MCU2 = 0x10
        self.datos_MCU1 = [0, 0, 0, 0, 0, 0, 0, 0, 0]
        self.datos_MCU2 = [0, 0, 0, 0, 0, 0, 0, 0, 0]
        self.contaa = 0
        self.contac = 0
        self.contae = 0
        self.contag = 0
        self.contai = 0
        self.contak = 0
        self.contam = 0
        self.contao = 0
        self.contaq = 0
        self.contas = 0

    def run(self):
        print("Starting " + self.name)
        # Formato de la trama del primer microcontrolador:
        # [Combustivel(valor), cuartos (A), baja (C), alta (E), stand(G), neutro(I), izq(K), der(M)]

        #Formato de la trama del segundo microcontrolador:
        # [Velocidad (valor), Temperatura (O), Aceite(Q), Aire(S)]

        datos_MCU1 = self.datos_MCU1
        datos_MCU2 = self.datos_MCU2
        contaa = self.contaa
        contac = self.contac
        contae = self.contae
        contag = self.contag
        contai = self.contai
        contak = self.contak
        contam = self.contam
        contao = self.contao
        contaq = self.contaq
        contas = self.contas

        while 1:
            time.sleep(0.1)
            self.datos_MCU1 = self.bus.read_i2c_block_data(self.addr_MCU1, 0, 8)
            self.datos_MCU2 = self.bus.read_i2c_block_data(self.addr_MCU1,0,)
            self.w.ui.Combustible.setValue(datos_MCU1[0])
            self.w.ui.Velocidad.setValue(datos_MCU2[0])

            if datos_MCU1[1] == 65:
                self.w.ui.Luz_Cuartos.show()
                contaa = contaa + 1
            elif contaa == 2:
                self.w.ui.Luz_Cuartos.hide()
                contaa = 0

            if datos_MCU1[2] == 67:
                self.w.ui.Luz_Baja.show()
                contac = contac + 1
            elif contac == 2:
                self.w.ui.Luz_Baja.show()
                contac = 0

            if datos_MCU1[3] == 69:
                self.w.ui.Luz_Alta.show()
                contae = contae + 1
            elif contae == 2:
                self.w.ui.Luz_Alta.hide()
                contae = 0

            if datos_MCU1[4] == 71:
                self.w.ui.Pal_Lat.show()
                contag = contag + 1
            elif contag == 2:
                self.w.ui.Pal_Lat.hide()
                contag = 0

            if datos_MCU1[5] == 73:
                self.w.ui.Ind_Neutral.show()
                contai = contai + 1
            elif contai == 2:
                self.w.ui.Ind_Neutral.hide()
                contai = 0

            if datos_MCU1[6] == 75:
                self.w.ui.Dir_Izq.show()
                contak = contak + 1
            elif contak == 2:
                self.w.ui.Dir_Izq.hide()
                contak = 0

            if datos_MCU1[7] == 77:
                self.w.ui.Dir_Der.show()
                contam = contam + 1
            elif contam == 2:
                self.w.ui.Dir_Der.hide()
                contam = 0

            if datos_MCU2[1] == 79:
                self.w.ui.Temp_Motor.show()
                contao = contao + 1
            elif contao == 2:
                self.w.ui.Temp_Motor.hide()
                contao = 0

            if datos_MCU2[2] == 81:
                self.w.ui.Bajo_Aceite.show()
                contaq = contaq + 1
            elif contaq == 2:
                self.w.ui.Bajo_Aceite.hide()
                contaq = 0

            if datos_MCU2[3] == 83:
                self.w.ui.Filtro_Aire.show()
                contas = contas + 1
            elif contas == 2:
                self.w.ui.Filtro_Aire.hide()
                contas = 0

            else:
                print("Valores no reconocibles")

            print("Finishing " + self.name)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = MyForm()
    thread1 = MyThread(w)
    thread1.start()
    w.exec()
    sys.exit(app.exec_())