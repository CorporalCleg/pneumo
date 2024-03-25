import form_v0_0_3 as form
import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication
import argparse

def application(graph=False):
    app = QApplication(sys.argv)
    window = QtWidgets.QMainWindow()
    
    ui = form.Ui_MainWindow()
    ui.setupUi(window, graph)
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--graph", help="set True to see graphics", type=bool, default=False)
    args = parser.parse_args()
    application(graph=False)