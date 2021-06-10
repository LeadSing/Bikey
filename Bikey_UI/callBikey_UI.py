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
    addr = 0x08
    data = 0
    data100 = 0

    def __init__(self, w):
        threading.Thread.__init__(self)
        self.w = w
        self.addr = 0x08
        self.data = 0

    def run(self):
        print("Starting " + self.name)
        while 1:
            time.sleep(0.5)
            self.data = self.bus.read_i2c_block_data(self.addr, 0, 2)
            data = self.data
            in_min = 0
            in_max = 255
            out_min = 0
            out_max = 100
            data100 = (data[0] - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
            self.w.ui.progressBar.setValue(data100)
            if data[1] <= 250:
                self.w.ui.label_2.hide()
            elif data[1] >= 251:
                self.w.ui.label_2.show()
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