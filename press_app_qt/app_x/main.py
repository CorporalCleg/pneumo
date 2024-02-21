import form_v0_0_3 as form
import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication

def application():
    app = QApplication(sys.argv)
    window = QtWidgets.QMainWindow()
    
    ui = form.Ui_MainWindow()
    ui.setupUi(window)
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    application()