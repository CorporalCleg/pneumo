from PyQt5 import QtWidgets, QtCore
from PyQt5.QtWidgets import QApplication, QMainWindow, QMenuBar, QMenu, QFileDialog

import sys

class Window(QMainWindow):
    def __init__(self):
        super(Window, self).__init__()

        self.setWindowTitle('Code editor')
        self.setGeometry(300, 250, 350, 200)

        self.text_edit = QtWidgets.QTextEdit(self)
        self.setCentralWidget(self.text_edit)
        self.create_menu_bar()

    def create_menu_bar(self):
        self.menuBar = QMenuBar()
        self.setMenuBar(self.menuBar)
        
        fileMenu = QMenu('&File', self)
        self.menuBar.addMenu(fileMenu)

        # open_file = fileMenu.addMenu('&open')
        # save_file = fileMenu.addMenu('&save')

        fileMenu.addAction('open', self.action_clicked)
        fileMenu.addAction('save', self.action_clicked)

    @QtCore.pyqtSlot()
    def action_clicked(self):
        action = self.sender()

        if action.text() == 'open':
            try:
                fname = QFileDialog.getOpenFileName(self)[0]
                with open(fname, 'r') as file:
                    data = ''
                    for line in file:
                        data += line
                    self.text_edit.setText(data)
            except FileNotFoundError:
                print('file not found')
        if action.text() == 'save':
            try:
                fname = QFileDialog.getSaveFileName(self)[0]
                with open(fname, 'w') as file:
                    data = self.text_edit.toPlainText()
                    file.write(data)
            except FileNotFoundError:
                print('file not found')




        elif action.text() == 'save':
            print('save')

def application():
    app = QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    application()