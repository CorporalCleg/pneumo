import form_v0_0_2 as form
import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication
import random
import time
import threading

def draw_numbers(field):
    print(str(random.randint(0, 10)))
    #time.sleep(2)

def application():
    app = QApplication(sys.argv)
    window = QtWidgets.QMainWindow()
    ui = form.Ui_MainWindow()
    ui.setupUi(window)
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    application()