import form
import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication



if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = QtWidgets.QMainWindow()
    ui = form.Ui_MainWindow()
    ui.setupUi(window)
    window.show()
    sys.exit(app.exec_())