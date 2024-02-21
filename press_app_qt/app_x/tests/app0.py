from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow

import sys

class Window(QMainWindow):
    def __init__(self):
        super(Window, self).__init__()

        self.setWindowTitle('simple')
        self.setGeometry(300, 250, 500, 500)

        #widgets on window
        self.main_text = QtWidgets.QLabel(self)
        self.main_text.setText('pu-pu-ru-r-r-r---r-r')
        self.main_text.move(100, 100)
        self.main_text.adjustSize()

        self.hidden_text = QtWidgets.QLabel(self)

        self.btn = QtWidgets.QPushButton(self)
        self.btn.setText('Tap')
        self.btn.move(250, 100)
        self.btn.setFixedWidth(50)
        self.btn.clicked.connect(self.add_label)

    def add_label(self):
        self.hidden_text.setText('pam-pam')
        self.hidden_text.move(300, 300)
        self.hidden_text.adjustSize()

def application():
    app = QApplication(sys.argv)
    window = Window()

    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    application()