#!/usr/bin/env python3

from PySide2 import QtCore, QtGui, QtWidgets

import mdieditor_rc


class Editor(QtWidgets.QTextEdit):

    documentNumber = 1

    def __init__(self, parent=None):
        QtWidgets.QTextEdit.__init__(self, parent)

        self.action = QtWidgets.QAction(self)
        self.action.setCheckable(True)
        self.action.triggered.connect(self.show)
        self.action.triggered.connect(self.setFocus)

        self.curFile = ""
        self.isUntitled = True
        self.fileFilters = self.tr("Text files (*.txt)\nAll files (*)")

        self.document().contentsChanged.connect(self.documentWasModified)

        self.setWindowIcon(QtGui.QPixmap(":/images/document.png"))
        self.setAttribute(QtCore.Qt.WA_DeleteOnClose)

    def windowMenuAction(self):
        return self.action

    def newFile(self):
        self.curFile = "document{0}.txt".format(Editor.documentNumber)
        self.setWindowTitle(self.curFile + "[*]")
        self.action.setText(self.curFile)
        self.isUntitled = True
        Editor.documentNumber += 1

    def open(self):
        fileName, _ = QtWidgets.QFileDialog.getOpenFileName(self, self.tr("Open"), ".", self.fileFilters)
        if not fileName:
            return False

        return self.openFile(fileName)

    def openFile(self, fileName):
        if self.readFile(fileName):
            self.setCurrentFile(fileName)
            return True
        else:
            return False

    def save(self):
        if self.isUntitled:
            return self.saveAs()
        else:
            return self.saveFile(self.curFile)

    def saveAs(self):
        fileName, _ = QtWidgets.QFileDialog.getSaveFileName(self, self.tr("Save As"), self.curFile, self.fileFilters)
        if not fileName:
            return False

        return self.saveFile(fileName)

    def sizeHint(self):
        return QtCore.QSize(72 * self.fontMetrics().width('x'), 25 * self.fontMetrics().lineSpacing())

    def closeEvent(self, event):
        if self.okToContinue():
            event.accept()
        else:
            event.ignore()

    def documentWasModified(self):
        self.setWindowModified(True)

    def okToContinue(self, ):
        if self.document().isModified():
            r = QtWidgets.QMessageBox.warning(self, self.tr("MDI Editor"),
                                              "File {0} has been modified.\nDo you want to save your changes?".format(self.strippedName(self.curFile)),
                                              QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.No | QtWidgets.QMessageBox.Cancel)

            if r == QtWidgets.QMessageBox.Yes:
                return self.save()
            elif r == QtWidgets.QMessageBox.Cancel:
                return False

        return True

    def saveFile(self, fileName):
        if self.writeFile(fileName):
            self.setCurrentFile(fileName)
            return True
        else:
            return False

    def setCurrentFile(self, fileName):
        self.curFile = fileName
        self.isUntitled = False
        self.action.setText(self.strippedName(self.curFile))
        self.document().setModified(False)
        self.setWindowTitle(self.strippedName(self.curFile) + "[*]")
        self.setWindowModified(False)

    def readFile(self, fileName):
        file = QtCore.QFile(fileName)
        if not file.open(QtCore.QIODevice.ReadOnly | QtCore.QIODevice.Text):
            QtWidgets.QMessageBox.warning(self, self.tr("MDI Editor"),
                                          self.tr("Cannot read file %1:\n%2.")
                                          .arg(file.fileName())
                                          .arg(file.errorString()))
            return False

        inf = QtCore.QTextStream(file)
        QtGui.QGuiApplication.setOverrideCursor(QtCore.Qt.WaitCursor)
        self.setPlainText(inf.readAll())
        QtGui.QGuiApplication.restoreOverrideCursor()
        return True

    def writeFile(self, fileName):
        file = QtCore.QFile(fileName)
        if not file.open(QtCore.QIODevice.WriteOnly | QtCore.QIODevice.Text):
            QtWidgets.QMessageBox.warning(self, self.tr("MDI Editor"),
                                          self.tr("Cannot write file %1:\n%2.")
                                          .arg(file.fileName())
                                          .arg(file.errorString()))
            return False

        out = QtCore.QTextStream(file)
        print("writeFile")
        print(fileName)
        s = self.document().toPlainText()
        print(s)
        QtGui.QGuiApplication.setOverrideCursor(QtCore.Qt.WaitCursor)
        out << self.document().toPlainText()
        QtGui.QGuiApplication.restoreOverrideCursor()
        return True

    def strippedName(self, fullFileName):
        return QtCore.QFileInfo(fullFileName).fileName()


class MainWindow(QtWidgets.QMainWindow):

    def __init__(self):
        super(MainWindow, self).__init__()

        self.workspace = QtWidgets.QMdiArea()
        self.setCentralWidget(self.workspace)
        self.workspace.subWindowActivated.connect(self.updateMenus)

        self.createActions()
        self.createMenus()
        self.createToolBars()
        self.createStatusBar()

        self.setWindowTitle(self.tr("MDI Editor"))
        self.setWindowIcon(QtGui.QPixmap(":/images/icon.png"))

    def newFile(self):
        editor = self.createEditor()
        editor.newFile()
        editor.show()

    def openFile(self, fileName):
        editor = self.createEditor()
        if editor.openFile(fileName):
            editor.show()
        else:
            editor.close()

    def closeEvent(self, event):
        self.workspace.closeAllSubWindows()
        if self.activeEditor():
            event.ignore()
        else:
            event.accept()

    def open(self):
        editor = self.createEditor()
        if editor.open():
            editor.show()
        else:
            editor.close()

    def save(self):
        if self.activeEditor():
            self.activeEditor().save()

    def saveAs(self):
        if self.activeEditor():
            self.activeEditor().saveAs()

    def cut(self):
        if self.activeEditor():
            self.activeEditor().cut()

    def copy(self):
        if self.activeEditor():
            self.activeEditor().copy()

    def paste(self):
        if self.activeEditor():
            self.activeEditor().paste()

    def about(self):
        QtWidgets.QMessageBox.about(self, self.tr("About MDI Editor"),
                                    self.tr("<h2>Editor 1.1</h2>"
                                            "<p>Copyright &copy; 2018 Software Inc."
                                            "<p>MDI Editor is a small application that demonstrates "
                                            "QWorkspace."))

    def updateMenus(self):
        hasEditor = self.activeEditor() != 0
        hasSelection = self.activeEditor() and self.activeEditor().textCursor().hasSelection()

        self.saveAction.setEnabled(hasEditor)
        self.saveAsAction.setEnabled(hasEditor)
        self.pasteAction.setEnabled(hasEditor)
        self.cutAction.setEnabled(hasSelection)
        self.copyAction.setEnabled(hasSelection)
        self.closeAction.setEnabled(hasEditor)
        self.closeAllAction.setEnabled(hasEditor)
        self.tileAction.setEnabled(hasEditor)
        self.cascadeAction.setEnabled(hasEditor)
        self.nextAction.setEnabled(hasEditor)
        self.previousAction.setEnabled(hasEditor)
        self.separatorAction.setVisible(hasEditor)

        if self.activeEditor():
            try:
                self.activeEditor().windowMenuAction().setChecked(True)
            except Exception:
                pass

    def createActions(self):
        self.newAction = QtWidgets.QAction(QtGui.QIcon(":/images/new.png"), self.tr("&New"), self)
        self.newAction.setShortcut(self.tr("Ctrl+N"))
        self.newAction.setStatusTip(self.tr("Create a new file"))
        self.newAction.triggered.connect(self.newFile)

        self.openAction = QtWidgets.QAction(QtGui.QIcon(":/images/open.png"), self.tr("&Open..."), self)
        self.openAction.setShortcut(self.tr("Ctrl+O"))
        self.openAction.setStatusTip(self.tr("Open an existing file"))
        self.openAction.triggered.connect(self.open)

        self.saveAction = QtWidgets.QAction(QtGui.QIcon(":/images/save.png"), self.tr("&Save"), self)
        self.saveAction.setShortcut(self.tr("Ctrl+S"))
        self.saveAction.setStatusTip(self.tr("Save the file to disk"))
        self.saveAction.triggered.connect(self.save)

        self.saveAsAction = QtWidgets.QAction(self.tr("Save &As..."), self)
        self.saveAsAction.setStatusTip(self.tr("Save the file under a new name"))
        self.saveAsAction.triggered.connect(self.saveAs)

        self.exitAction = QtWidgets.QAction(self.tr("E&xit"), self)
        self.exitAction.setShortcut(self.tr("Ctrl+Q"))
        self.exitAction.setStatusTip(self.tr("Exit the application"))
        self.exitAction.triggered.connect(self.close)

        self.cutAction = QtWidgets.QAction(QtGui.QIcon(":/images/cut.png"), self.tr("Cu&t"), self)
        self.cutAction.setShortcut(self.tr("Ctrl+X"))
        self.cutAction.setStatusTip(self.tr("Cut the current selection to the clipboard"))
        self.cutAction.triggered.connect(self.cut)

        self.copyAction = QtWidgets.QAction(QtGui.QIcon(":/images/copy.png"), self.tr("&Copy"), self)
        self.copyAction.setShortcut(self.tr("Ctrl+C"))
        self.copyAction.setStatusTip(self.tr("Copy the current selection to the clipboard"))
        self.copyAction.triggered.connect(self.copy)

        self.pasteAction = QtWidgets.QAction(QtGui.QIcon(":/images/paste.png"), self.tr("&Paste"), self)
        self.pasteAction.setShortcut(self.tr("Ctrl+V"))
        self.pasteAction.setStatusTip(self.tr("Paste the clipboard's contents at the cursor position"))
        self.pasteAction.triggered.connect(self.paste)

        self.closeAction = QtWidgets.QAction(self.tr("Cl&ose"), self)
        self.closeAction.setShortcut(self.tr("Ctrl+F4"))
        self.closeAction.setStatusTip(self.tr("Close the active window"))
        self.closeAction.triggered.connect(self.workspace.closeActiveSubWindow)

        self.closeAllAction = QtWidgets.QAction(self.tr("Close &All"), self)
        self.closeAllAction.setStatusTip(self.tr("Close all the windows"))
        self.closeAllAction.triggered.connect(self.workspace.closeAllSubWindows)

        self.tileAction = QtWidgets.QAction(self.tr("&Tile"), self)
        self.tileAction.setStatusTip(self.tr("Tile the windows"))
        self.tileAction.triggered.connect(self.workspace.tileSubWindows)

        self.cascadeAction = QtWidgets.QAction(self.tr("&Cascade"), self)
        self.cascadeAction.setStatusTip(self.tr("Cascade the windows"))
        self.cascadeAction.triggered.connect(self.workspace.cascadeSubWindows)

        self.nextAction = QtWidgets.QAction(self.tr("Ne&xt"), self)
        self.nextAction.setShortcut(self.tr("Ctrl+F6"))
        self.nextAction.setStatusTip(self.tr("Move the focus to the next window"))
        self.nextAction.triggered.connect(self.workspace.activateNextSubWindow)

        self.previousAction = QtWidgets.QAction(self.tr("Pre&vious"), self)
        self.previousAction.setShortcut(self.tr("Ctrl+Shift+F6"))
        self.previousAction.setStatusTip(self.tr("Move the focus to the previous window"))
        self.previousAction.triggered.connect(self.workspace.activatePreviousSubWindow)

        self.separatorAction = QtWidgets.QAction(self)
        self.separatorAction.setSeparator(True)

        self.aboutAction = QtWidgets.QAction(self.tr("&About"), self)
        self.aboutAction.setStatusTip(self.tr("Show the application's About box"))
        self.aboutAction.triggered.connect(self.about)

        self.aboutQtAction = QtWidgets.QAction(self.tr("About &Qt"), self)
        self.aboutQtAction.setStatusTip(self.tr("Show the Qt library's About box"))
        self.aboutQtAction.triggered.connect(qApp.aboutQt)

        self.windowActionGroup = QtWidgets.QActionGroup(self)

    def createMenus(self):
        self.fileMenu = self.menuBar().addMenu(self.tr("&File"))
        self.fileMenu.addAction(self.newAction)
        self.fileMenu.addAction(self.openAction)
        self.fileMenu.addAction(self.saveAction)
        self.fileMenu.addAction(self.saveAsAction)
        self.fileMenu.addSeparator()
        self.fileMenu.addAction(self.exitAction)

        self.editMenu = self.menuBar().addMenu(self.tr("&Edit"))
        self.editMenu.addAction(self.cutAction)
        self.editMenu.addAction(self.copyAction)
        self.editMenu.addAction(self.pasteAction)

        self.windowMenu = self.menuBar().addMenu(self.tr("&Window"))
        self.windowMenu.addAction(self.closeAction)
        self.windowMenu.addAction(self.closeAllAction)
        self.windowMenu.addSeparator()
        self.windowMenu.addAction(self.tileAction)
        self.windowMenu.addAction(self.cascadeAction)
        self.windowMenu.addSeparator()
        self.windowMenu.addAction(self.nextAction)
        self.windowMenu.addAction(self.previousAction)
        self.windowMenu.addAction(self.separatorAction)

        self.menuBar().addSeparator()

        self.helpMenu = self.menuBar().addMenu(self.tr("&Help"))
        self.helpMenu.addAction(self.aboutAction)
        self.helpMenu.addAction(self.aboutQtAction)

    def createToolBars(self):
        self.fileToolBar = self.addToolBar(self.tr("File"))
        self.fileToolBar.addAction(self.newAction)
        self.fileToolBar.addAction(self.openAction)
        self.fileToolBar.addAction(self.saveAction)

        self.editToolBar = self.addToolBar(self.tr("Edit"))
        self.editToolBar.addAction(self.cutAction)
        self.editToolBar.addAction(self.copyAction)
        self.editToolBar.addAction(self.pasteAction)

    def createStatusBar(self):
        readyLabel = QtWidgets.QLabel(self.tr(" Ready"))
        self.statusBar().addWidget(readyLabel, 1)

    def createEditor(self):
        editor = Editor()
        editor.copyAvailable.connect(self.cutAction.setEnabled)
        editor.copyAvailable.connect(self.copyAction.setEnabled)

        self.workspace.addSubWindow(editor)
        self.windowMenu.addAction(editor.windowMenuAction())
        self.windowActionGroup.addAction(editor.windowMenuAction())

        return editor

    def activeEditor(self):
        return Editor(self.workspace.activeSubWindow())


if __name__ == '__main__':

    import sys

    app = QtWidgets.QApplication(sys.argv)
    mainWin = MainWindow()

    args = app.arguments()

    if len(args) > 1:
        for i in range(1, len(args)):
            mainWin.openFile(args[i])
    else:
        mainWin.newFile()

    mainWin.show()
    sys.exit(app.exec_())
