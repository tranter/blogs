#!/usr/bin/env python3

from PySide2 import QtCore, QtGui, QtWidgets

import spreadsheet_rc
from ui_gotocelldialog import Ui_GoToCellDialog
from ui_sortdialog import Ui_SortDialog


class SortDialog(QtWidgets.QDialog):

    def __init__(self, parent=None):
        QtWidgets.QDialog.__init__(self, parent)
        self.ui = Ui_SortDialog()
        self.ui.setupUi(self)

        self.ui.secondaryGroupBox.hide()
        self.ui.tertiaryGroupBox.hide()
        self.layout().setSizeConstraint(QtWidgets.QLayout.SetFixedSize)
        self.setColumnRange('A', 'Z')

    def setColumnRange(self, first, last):
        self.ui.primaryColumnCombo.clear()
        self.ui.secondaryColumnCombo.clear()
        self.ui.tertiaryColumnCombo.clear()

        self.ui.secondaryColumnCombo.addItem(self.tr("None"))
        self.ui.tertiaryColumnCombo.addItem(self.tr("None"))

        self.ui.primaryColumnCombo.setMinimumSize(self.ui.secondaryColumnCombo.sizeHint())

        ch = first
        while ch <= last:
            self.ui.primaryColumnCombo.addItem(ch)
            self.ui.secondaryColumnCombo.addItem(ch)
            self.ui.tertiaryColumnCombo.addItem(ch)
            ch = chr(ord(ch) + 1)


class GoToCellDialog(QtWidgets.QDialog):

    def __init__(self, parent=None):
        QtWidgets.QDialog.__init__(self, parent)
        self.ui = Ui_GoToCellDialog()
        self.ui.setupUi(self)

        regExp = QtCore.QRegExp("[A-Za-z][1-9][0-9]{0,2}")
        self.ui.lineEdit.setValidator(QtGui.QRegExpValidator(regExp, self))

        self.ui.okButton.clicked.connect(self.accept)
        self.ui.cancelButton.clicked.connect(self.reject)
        self.ui.lineEdit.textChanged.connect(self.textChanged)

    def textChanged(self):
        self.ui.okButton.setEnabled(self.ui.lineEdit.hasAcceptableInput())


class FindDialog(QtWidgets.QDialog):
    findNext = QtCore.Signal(str, QtCore.Qt.CaseSensitivity)
    findPrevious = QtCore.Signal(str, QtCore.Qt.CaseSensitivity)

    def __init__(self, parent=None):
        QtWidgets.QDialog.__init__(self, parent)

        self.label = QtWidgets.QLabel(self.tr("Find &what:"))
        self.lineEdit = QtWidgets.QLineEdit()
        self.label.setBuddy(self.lineEdit)

        self.caseCheckBox = QtWidgets.QCheckBox(self.tr("Match &case"))
        self.backwardCheckBox = QtWidgets.QCheckBox(self.tr("Search &backward"))

        self.findButton = QtWidgets.QPushButton(self.tr("&Find"))
        self.findButton.setDefault(True)
        self.findButton.setEnabled(False)

        self.closeButton = QtWidgets.QPushButton(self.tr("Close"))

        self.lineEdit.textChanged.connect(self.enableFindButton)
        self.findButton.clicked.connect(self.findClicked)
        self.closeButton.clicked.connect(self.close)

        topLeftLayout = QtWidgets.QHBoxLayout()
        topLeftLayout.addWidget(self.label)
        topLeftLayout.addWidget(self.lineEdit)

        leftLayout = QtWidgets.QVBoxLayout()
        leftLayout.addLayout(topLeftLayout)
        leftLayout.addWidget(self.caseCheckBox)
        leftLayout.addWidget(self.backwardCheckBox)

        rightLayout = QtWidgets.QVBoxLayout()
        rightLayout.addWidget(self.findButton)
        rightLayout.addWidget(self.closeButton)
        rightLayout.addStretch()

        mainLayout = QtWidgets.QHBoxLayout()
        mainLayout.addLayout(leftLayout)
        mainLayout.addLayout(rightLayout)
        self.setLayout(mainLayout)

        self.setWindowTitle(self.tr("Find"))
        self.setFixedHeight(self.sizeHint().height())

    def findClicked(self):
        text = self.lineEdit.text()
        cs = QtCore.Qt.CaseSensitive if self.caseCheckBox.isChecked() else QtCore.Qt.CaseInsensitive

        if self.backwardCheckBox.isChecked():
            self.findPrevious.emit(text, cs)
        else:
            self.findNext.emit(text, cs)

    def enableFindButton(self, text):
        self.findButton.setEnabled(text != "")


class Cell(QtWidgets.QTableWidgetItem):
    Invalid = "Invalid"

    def __init__(self, parent=None):
        QtWidgets.QTableWidgetItem.__init__(self, parent)
        self.setDirty()

#    def clone(self):
#        return Cell(self)
#        return self

    def setData(self, role, value):
        QtWidgets.QTableWidgetItem.setData(self, role, value)
        # super().setData(role, value)
        if role == QtCore.Qt.EditRole:
            self.setDirty()

    def data(self, role):
        if role == QtCore.Qt.DisplayRole:
            if self.value() != self.Invalid:
                return self.value()
            else:
                return "####"
        elif role == QtCore.Qt.TextAlignmentRole:
            if type(self.value()) is str:
                return int(QtCore.Qt.AlignLeft | QtCore.Qt.AlignVCenter)
            else:
                return int(QtCore.Qt.AlignRight | QtCore.Qt.AlignVCenter)
        else:
            # return super().data(role)
            return QtWidgets.QTableWidgetItem.data(self, role)

    def setFormula(self, formula):
        self.setData(QtCore.Qt.EditRole, formula)

    def formula(self):
        return self.data(QtCore.Qt.EditRole)

    def setDirty(self):
        self.cacheIsDirty = True

    def value(self):
        if self.cacheIsDirty:
            self.cacheIsDirty = False

            formulaStr = self.formula()
            if formulaStr.startswith("'"):
                self.cachedValue = formulaStr.mid(1)
            elif formulaStr.startswith('='):
                self.cachedValue = self.Invalid
                expr = formulaStr[1:]
                expr.replace(" ", "")
                pos = 0
                self.cachedValue = self.evalExpression(expr, pos)
                if expr[pos] != QtCore.QChar.Null:
                    self.cachedValue = self.Invalid
            else:
                try:
                    d = float(formulaStr)
                    self.cachedValue = d
                except Exception:
                    self.cachedValue = formulaStr

        return self.cachedValue

    def evalExpression(self, str, pos):
        result = self.evalTerm(str, pos)
        while str[pos] != QtCore.QChar.Null:
            op = str[pos]
            if op != '+' and op != '-':
                return result
            ++pos

            term = self.evalTerm(str, pos)
            if result.type() == QtCore.QVariant.Double and term.type() == QtCore.QVariant.Double:
                if op == '+':
                    result = result.toDouble() + term.toDouble()
                else:
                    result = result.toDouble() - term.toDouble()
            else:
                result = self.Invalid
                return result

    def evalTerm(self, str, pos):
        result = self.evalFactor(str, pos)
        while str[pos] != QtCore.QChar.Null:
            op = str[pos]
            if op != '*' and op != '/':
                return result
            ++pos

            factor = self.evalFactor(str, pos)
            if result.type() == QtCore.QVariant.Double and factor.type() == QtCore.QVariant.Double:
                if op == '*':
                    result = result.toDouble() * factor.toDouble()
                else:
                    if factor.toDouble() == 0.0:
                        result = self.Invalid
                    else:
                        result = result.toDouble() / factor.toDouble()
            else:
                result = self.Invalid
        return result

    def evalFactor(self, str, pos):
        result = ""
        negative = False

        if str[pos] == '-':
            negative = True
            ++pos

        if str[pos] == '(':
            ++pos
            result = self.evalExpression(str, pos)
            if str[pos] != ')':
                result = self.Invalid
            ++pos
        else:
            regExp = QtCore.QRegExp("[A-Za-z][1-9][0-9]{0,2}")
            token = ""

            while str[pos].isLetterOrNumber() or str[pos] == '.':
                token += str[pos]
                ++pos

            if regExp.exactMatch(token):
                column = token[0].toUpper() - 'A'
                row = token.midRef(1).toInt() - 1

                c = self.tableWidget().item(row, column)

                if c:
                    result = c.value()
                else:
                    result = 0.0

            else:
                ok = False
                result = token.toDouble(ok)
                if not ok:
                    result = self.Invalid

        if negative:
            if result.type() == QtCore.QVariant.Double:
                result = -result.toDouble()
            else:
                result = self.Invalid

        return result


class SpreadsheetCompare():
    keyCount = 3
    keys = [0, 0, 0]
    ascending = [False, False, False]

    def operator(self, row1, row2):
        for i in range(0, self.keyCount):
            column = self.keys[i]
            if column != -1:
                if row1[column] != row2[column]:
                    if self.ascending[i]:
                        return row1[column] < row2[column]
                    else:
                        return row1[column] > row2[column]
        return False


class Spreadsheet(QtWidgets.QTableWidget):
    modified = QtCore.Signal()
    MagicNumber = 0x7F51C883
    RowCount = 999
    ColumnCount = 26

    def __init__(self, parent=None):
        QtWidgets.QTableWidget.__init__(self, parent)
        self.autoRecalc = True
        self.setSelectionMode(QtWidgets.QAbstractItemView.ContiguousSelection)
        self.itemChanged.connect(self.somethingChanged)
        self.clear()

    def currentLocation(self):
        return chr(ord('A') + self.currentColumn()) + str(self.currentRow() + 1)

    def currentFormula(self):
        return self.formula(self.currentRow(), self.currentColumn())

    def selectedRange(self):
        ranges = self.selectedRanges()
        if not ranges:
            return QtWidgets.QTableWidgetSelectionRange()
        return ranges[0]

    def clear(self):
        self.setRowCount(0)
        self.setColumnCount(0)
        self.setRowCount(self.RowCount)
        self.setColumnCount(self.ColumnCount)

        for i in range(0, self.ColumnCount):
            item = QtWidgets.QTableWidgetItem()
            item.setText(chr(ord('A') + i))
            self.setHorizontalHeaderItem(i, item)

        self.setCurrentCell(0, 0)

    def readFile(self, fileName):
        file = QtCore.QFile(fileName)
        if not file.open(QtCore.QIODevice.ReadOnly):
            QtWidgets.QMessageBox.warning(self,
                                          self.tr("Spreadsheet"),
                                          self.tr("Cannot read file %1:\n%2.").arg(file.fileName(),
                                                                                   file.errorString()))
            return False

        inp = QtCore.QDataStream(file)
        inp.setVersion(QtCore.QDataStream.Qt_4_1)

        magic = inp.readInt32()
        if magic != self.MagicNumber:
            QtWidgets.QMessageBox.warning(self, self.tr("Spreadsheet"), self.tr("The file is not a Spreadsheet file."))
            return False

        self.clear()

        row = 0
        column = 0
        str = ""

        QtWidgets.QApplication.setOverrideCursor(QtCore.Qt.WaitCursor)
        while not inp.atEnd():
            # inp >> row >> column >> str
            inp >> row
            inp >> column
            inp >> str
            self.setFormula(row, column, str)

        QtWidgets.QApplication.restoreOverrideCursor()
        return True

    def writeFile(self, fileName):
        file = QtCore.QFile(fileName)
        if not file.open(QtCore.QIODevice.WriteOnly):
            QtWidgets.QMessageBox.warning(self,
                                          self.tr("Spreadsheet"),
                                          self.tr("Cannot write file {0}:\n{1}.").format(file.fileName(),
                                                                                         file.errorString()))
            return False

        out = QtCore.QDataStream(file)
        out.setVersion(QtCore.QDataStream.Qt_4_1)

        out << int(self.MagicNumber)

        QtGui.QGuiApplication.setOverrideCursor(QtCore.Qt.WaitCursor)
        for row in range(0, self.RowCount):
            for column in range(0, self.ColumnCount):
                str = self.formula(row, column)
                if str != "":
                    out << int(row)
                    out << int(column)
                    out << str

        QtGui.QGuiApplication.restoreOverrideCursor()
        return True

    def sort(self, compare):
        srange = self.selectedRange()
        rows = []

        for i in range(0, srange.rowCount()):
            row = []
            for j in range(0, srange.columnCount()):
                row.append(self.formula(srange.topRow() + i, srange.leftColumn() + j))
            rows.append(row)

        # qStableSort(rows.begin(), rows.end(), compare)

        for i in range(0, srange.rowCount()):
            for j in range(0, srange.columnCount()):
                self.setFormula(srange.topRow() + i, srange.leftColumn() + j, rows[i][j])

        self.clearSelection()
        self.somethingChanged()

    def cut(self):
        self.copy()
        self.delete()

    def copy(self):
        srange = self.selectedRange()
        str = ""

        for i in range(0, srange.rowCount()):
            if i > 0:
                str += "\n"
            for j in range(0, srange.columnCount()):
                if j > 0:
                    str += "\t"
                str += self.formula(srange.topRow() + i, srange.leftColumn() + j)

        QtWidgets.QApplication.clipboard().setText(str)

    def paste(self):
        srange = self.selectedRange()
        str = QtWidgets.QApplication.clipboard().text()
        rows = str.split('\n')
        numRows = len(rows)
        numColumns = rows[0].count('\t') + 1

        if srange.rowCount() * srange.columnCount() != 1 and (srange.rowCount() != numRows or srange.columnCount() != numColumns):
            QtWidgets.QMessageBox.information(self,
                                              self.tr("Spreadsheet"),
                                              self.tr("The information cannot be pasted because the "
                                                      "copy and paste areas aren't the same size."))
            return

        for i in range(0, numRows):
            columns = rows[i].split('\t')
            for j in range(0, numColumns):
                row = srange.topRow() + i
                column = srange.leftColumn() + j
                if row < self.RowCount and column < self.ColumnCount:
                    self.setFormula(row, column, columns[j])

        self.somethingChanged()

    def delete(self):
        for item in self.selectedItems():
            del item

    def selectCurrentRow(self):
        self.selectRow(self.currentRow())

    def selectCurrentColumn(self):
        self.selectColumn(self.currentColumn())

    def recalculate(self):
        for row in range(0, self.RowCount):
            for column in range(0, self.ColumnCount):
                if self.cell(row, column):
                    self.cell(row, column).setDirty()
        self.viewport().update()

    def autoRecalculate(self):
        return self.autoRecalc

    def setAutoRecalculate(self, recalc):
        self.autoRecalc = recalc
        if self.autoRecalc:
            self.recalculate()

    def findNext(self, str, cs):
        row = self.currentRow()
        column = self.currentColumn() + 1

        while row < self.RowCount:
            while column < self.ColumnCount:
                if ((cs == QtCore.Qt.CaseSensitive) and str in self.text(row, column)) or \
                   ((cs == QtCore.Qt.CaseInsensitive) and str.upper() in self.text(row, column).upper()):
                    self.clearSelection()
                    self.setCurrentCell(row, column)
                    self.activateWindow()
                    return
                column += 1
            column = 0
            row += 1

        QtWidgets.QApplication.beep()

    def findPrevious(self, str, cs):
        row = self.currentRow()
        column = self.currentColumn() - 1

        while row >= 0:
            while column >= 0:
                if ((cs == QtCore.Qt.CaseSensitive) and str in self.text(row, column)) or \
                   ((cs == QtCore.Qt.CaseInsensitive) and str.upper() in self.text(row, column).upper()):
                    self.clearSelection()
                    self.setCurrentCell(row, column)
                    self.activateWindow()
                    return
                column -= 1
            column = self.ColumnCount - 1
            row -= 1

        QtWidgets.QApplication.beep()

    def somethingChanged(self):
        if self.autoRecalc:
            self.recalculate()
        self.modified.emit()

    def cell(self, row, column):
        return Cell(self.item(row, column))

    def setFormula(self, row, column, formula):
        c = self.cell(row, column)
        if not c:
            c = Cell()
            self.setItem(row, column, c)

        c.setFormula(formula)

    def formula(self, row, column):
        c = self.cell(row, column)
        if c:
            return c.formula()
        else:
            return ""

    def text(self, row, column):
        c = self.cell(row, column)
        if c:
            return c.text()
        else:
            return ""


class MainWindow(QtWidgets.QMainWindow):
    MaxRecentFiles = 5

    def __init__(self):
        super(MainWindow, self).__init__()
        self.spreadsheet = Spreadsheet()
        self.setCentralWidget(self.spreadsheet)

        self.recentFiles = []

        self.createActions()
        self.createMenus()
        self.createContextMenu()
        self.createToolBars()
        self.createStatusBar()

        self.readSettings()

        self.findDialog = 0

        self.setWindowIcon(QtGui.QIcon(":/images/icon.png"))
        self.setCurrentFile("")

    def closeEvent(self, event):
        if self.okToContinue():
            self.writeSettings()
            event.accept()
        else:
            event.ignore()

    def newFile(self):
        if self.okToContinue():
            self.spreadsheet.clear()
            self.setCurrentFile("")

    def open(self):
        if self.okToContinue():
            fileName, _ = QtWidgets.QFileDialog.getOpenFileName(self,
                                                                self.tr("Open Spreadsheet"),
                                                                ".",
                                                                self.tr("Spreadsheet files (*.sp)"))
        if fileName != "":
            self.loadFile(fileName)

    def save(self):
        if self.curFile == "":
            return self.saveAs()
        else:
            return self.saveFile(self.curFile)

    def saveAs(self):
        fileName, _ = QtWidgets.QFileDialog.getSaveFileName(self,
                                                            self.tr("Save Spreadsheet"), ".",
                                                            self.tr("Spreadsheet files (*.sp)"))
        if fileName == "":
            return False

        return self.saveFile(fileName)

    def find(self):
        if not self.findDialog:
            self.findDialog = FindDialog(self)
            self.findDialog.findNext.connect(self.spreadsheet.findNext)
            self.findDialog.findPrevious.connect(self.spreadsheet.findPrevious)

        self.findDialog.show()
        self.findDialog.activateWindow()

    def goToCell(self):
        dialog = GoToCellDialog(self)
        if dialog.exec_():
            str = dialog.ui.lineEdit.text().upper()
            self.spreadsheet.setCurrentCell(int(str[1:]) - 1, ord(str[0]) - ord('A'))

    def sort(self):
        dialog = SortDialog(self)
        srange = self.spreadsheet.selectedRange()
        dialog.setColumnRange(chr(ord('A') + srange.leftColumn()), chr(ord('A') + srange.rightColumn()))

        if dialog.exec_():
            compare = SpreadsheetCompare()
            compare.keys[0] = dialog.ui.primaryColumnCombo.currentIndex()
            compare.keys[1] = dialog.ui.secondaryColumnCombo.currentIndex() - 1
            compare.keys[2] = dialog.ui.tertiaryColumnCombo.currentIndex() - 1
            compare.ascending[0] = (dialog.ui.primaryOrderCombo.currentIndex() == 0)
            compare.ascending[1] = (dialog.ui.secondaryOrderCombo.currentIndex() == 0)
            compare.ascending[2] = (dialog.ui.tertiaryOrderCombo.currentIndex() == 0)
            self.spreadsheet.sort(compare)

    def about(self):
        QtWidgets.QMessageBox.about(self, self.tr("About Spreadsheet"),
                                    self.tr("<h2>Spreadsheet 1.1</h2>"
                                            "<p>Copyright &copy; 2006-2018 Software Inc."
                                            "<p>Spreadsheet is a small application that "
                                            "demonstrates QAction, QMainWindow, QMenuBar, "
                                            "QStatusBar, QTableWidget, QToolBar, and many other "
                                            "Qt classes."))

    def openRecentFile(self):
        if self.okToContinue():
            action = QtWidgets.QAction(self.sender())
            if action:
                self.loadFile(action.data().toString())

    def updateStatusBar(self):
        self.locationLabel.setText(self.spreadsheet.currentLocation())
        self.formulaLabel.setText(self.spreadsheet.currentFormula())

    def spreadsheetModified(self):
        self.setWindowModified(True)
        self.updateStatusBar()

    def createActions(self):
        self.newAction = QtWidgets.QAction(self.tr("&New"), self)
        self.newAction.setIcon(QtGui.QIcon(":/images/new.png"))
        self.newAction.setShortcut(self.tr("Ctrl+N"))
        self.newAction.setStatusTip(self.tr("Create a new spreadsheet file"))
        self.newAction.triggered.connect(self.newFile)

        self.openAction = QtWidgets.QAction(self.tr("&Open..."), self)
        self.openAction.setIcon(QtGui.QIcon(":/images/open.png"))
        self.openAction.setShortcut(self.tr("Ctrl+O"))
        self.openAction.setStatusTip(self.tr("Open an existing spreadsheet file"))
        self.openAction.triggered.connect(self.open)

        self.saveAction = QtWidgets.QAction(self.tr("&Save"), self)
        self.saveAction.setIcon(QtGui.QIcon(":/images/save.png"))
        self.saveAction.setShortcut(self.tr("Ctrl+S"))
        self.saveAction.setStatusTip(self.tr("Save the spreadsheet to disk"))
        self.saveAction.triggered.connect(self.save)

        self.saveAsAction = QtWidgets.QAction(self.tr("Save &As..."), self)
        self.saveAsAction.setStatusTip(self.tr("Save the spreadsheet under a new name"))
        self.saveAsAction.triggered.connect(self.saveAs)

        self.recentFileActions = []
        for i in range(0, self.MaxRecentFiles):
            self.recentFileActions.append(QtWidgets.QAction(self))
            self.recentFileActions[i].setVisible(False)
            self.recentFileActions[i].triggered.connect(self.openRecentFile)

        self.exitAction = QtWidgets.QAction(self.tr("E&xit"), self)
        self.exitAction.setShortcut(self.tr("Ctrl+Q"))
        self.exitAction.setStatusTip(self.tr("Exit the application"))
        self.exitAction.triggered.connect(self.close)

        self.cutAction = QtWidgets.QAction(self.tr("Cu&t"), self)
        self.cutAction.setIcon(QtGui.QIcon(":/images/cut.png"))
        self.cutAction.setShortcut(self.tr("Ctrl+X"))
        self.cutAction.setStatusTip(self.tr("Cut the current selection's contents to the clipboard"))
        self.cutAction.triggered.connect(self.spreadsheet.cut)

        self.copyAction = QtWidgets.QAction(self.tr("&Copy"), self)
        self.copyAction.setIcon(QtGui.QIcon(":/images/copy.png"))
        self.copyAction.setShortcut(self.tr("Ctrl+C"))
        self.copyAction.setStatusTip(self.tr("Copy the current selection's contents to the clipboard"))
        self.copyAction.triggered.connect(self.spreadsheet.copy)

        self.pasteAction = QtWidgets.QAction(self.tr("&Paste"), self)
        self.pasteAction.setIcon(QtGui.QIcon(":/images/paste.png"))
        self.pasteAction.setShortcut(self.tr("Ctrl+V"))
        self.pasteAction.setStatusTip(self.tr("Paste the clipboard's contents into the current selection"))
        self.pasteAction.triggered.connect(self.spreadsheet.paste)

        self.deleteAction = QtWidgets.QAction(self.tr("&Delete"), self)
        self.deleteAction.setShortcut(self.tr("Del"))
        self.deleteAction.setStatusTip(self.tr("Delete the current selection's contents"))
        self.deleteAction.triggered.connect(self.spreadsheet.delete)

        self.selectRowAction = QtWidgets.QAction(self.tr("&Row"), self)
        self.selectRowAction.setStatusTip(self.tr("Select all the cells in the current row"))
        self.selectRowAction.triggered.connect(self.spreadsheet.selectCurrentRow)

        self.selectColumnAction = QtWidgets.QAction(self.tr("&Column"), self)
        self.selectColumnAction.setStatusTip(self.tr("Select all the cells in the current column"))
        self.selectColumnAction.triggered.connect(self.spreadsheet.selectCurrentColumn)

        self.selectAllAction = QtWidgets.QAction(self.tr("&All"), self)
        self.selectAllAction.setShortcut(self.tr("Ctrl+A"))
        self.selectAllAction.setStatusTip(self.tr("Select all the cells in the spreadsheet"))
        self.selectAllAction.triggered.connect(self.spreadsheet.selectAll)

        self.findAction = QtWidgets.QAction(self.tr("&Find..."), self)
        self.findAction.setIcon(QtGui.QIcon(":/images/find.png"))
        self.findAction.setShortcut(self.tr("Ctrl+F"))
        self.findAction.setStatusTip(self.tr("Find a matching cell"))
        self.findAction.triggered.connect(self.find)

        self.goToCellAction = QtWidgets.QAction(self.tr("&Go to Cell..."), self)
        self.goToCellAction.setIcon(QtGui.QIcon(":/images/gotocell.png"))
        self.goToCellAction.setShortcut(self.tr("F5"))
        self.goToCellAction.setStatusTip(self.tr("Go to the specified cell"))
        self.goToCellAction.triggered.connect(self.goToCell)

        self.recalculateAction = QtWidgets.QAction(self.tr("&Recalculate"), self)
        self.recalculateAction.setShortcut(self.tr("F9"))
        self.recalculateAction.setStatusTip(self.tr("Recalculate all the spreadsheet's formulas"))
        self.recalculateAction.triggered.connect(self.spreadsheet.recalculate)

        self.sortAction = QtWidgets.QAction(self.tr("&Sort..."), self)
        self.sortAction.setStatusTip(self.tr("Sort the selected cells or all the cells"))
        self.sortAction.triggered.connect(self.sort)

        self.showGridAction = QtWidgets.QAction(self.tr("&Show Grid"), self)
        self.showGridAction.setCheckable(True)
        self.showGridAction.setChecked(self.spreadsheet.showGrid())
        self.showGridAction.setStatusTip(self.tr("Show or hide the spreadsheet's grid"))
        self.showGridAction.toggled.connect(self.spreadsheet.setShowGrid)

        self.autoRecalcAction = QtWidgets.QAction(self.tr("&Auto-Recalculate"), self)
        self.autoRecalcAction.setCheckable(True)
        self.autoRecalcAction.setChecked(self.spreadsheet.autoRecalculate())
        self.autoRecalcAction.setStatusTip(self.tr("Switch auto-recalculation on or off"))
        self.autoRecalcAction.toggled.connect(self.spreadsheet.setAutoRecalculate)

        self.aboutAction = QtWidgets.QAction(self.tr("&About"), self)
        self.aboutAction.setStatusTip(self.tr("Show the application's About box"))
        self.aboutAction.triggered.connect(self.about)

        self.aboutQtAction = QtWidgets.QAction(self.tr("About &Qt"), self)
        self.aboutQtAction.setStatusTip(self.tr("Show the Qt library's About box"))
        self.aboutQtAction.triggered.connect(qApp.aboutQt)

    def createMenus(self):
        fileMenu = self.menuBar().addMenu(self.tr("&File"))
        fileMenu.addAction(self.newAction)
        fileMenu.addAction(self.openAction)
        fileMenu.addAction(self.saveAction)
        fileMenu.addAction(self.saveAsAction)
        self.separatorAction = fileMenu.addSeparator()
        for i in range(0, self.MaxRecentFiles):
            fileMenu.addAction(self.recentFileActions[i])
        fileMenu.addSeparator()
        fileMenu.addAction(self.exitAction)

        editMenu = self.menuBar().addMenu(self.tr("&Edit"))
        editMenu.addAction(self.cutAction)
        editMenu.addAction(self.copyAction)
        editMenu.addAction(self.pasteAction)
        editMenu.addAction(self.deleteAction)

        selectSubMenu = editMenu.addMenu(self.tr("&Select"))
        selectSubMenu.addAction(self.selectRowAction)
        selectSubMenu.addAction(self.selectColumnAction)
        selectSubMenu.addAction(self.selectAllAction)

        editMenu.addSeparator()
        editMenu.addAction(self.findAction)
        editMenu.addAction(self.goToCellAction)

        toolsMenu = self.menuBar().addMenu(self.tr("&Tools"))
        toolsMenu.addAction(self.recalculateAction)
        toolsMenu.addAction(self.sortAction)

        optionsMenu = self.menuBar().addMenu(self.tr("&Options"))
        optionsMenu.addAction(self.showGridAction)
        optionsMenu.addAction(self.autoRecalcAction)

        self.menuBar().addSeparator()

        helpMenu = self.menuBar().addMenu(self.tr("&Help"))
        helpMenu.addAction(self.aboutAction)
        helpMenu.addAction(self.aboutQtAction)

    def createContextMenu(self):
        self.spreadsheet.addAction(self.cutAction)
        self.spreadsheet.addAction(self.copyAction)
        self.spreadsheet.addAction(self.pasteAction)
        self.spreadsheet.setContextMenuPolicy(QtCore.Qt.ActionsContextMenu)

    def createToolBars(self):
        fileToolBar = self.addToolBar(self.tr("&File"))
        fileToolBar.addAction(self.newAction)
        fileToolBar.addAction(self.openAction)
        fileToolBar.addAction(self.saveAction)

        editToolBar = self.addToolBar(self.tr("&Edit"))
        editToolBar.addAction(self.cutAction)
        editToolBar.addAction(self.copyAction)
        editToolBar.addAction(self.pasteAction)
        editToolBar.addSeparator()
        editToolBar.addAction(self.findAction)
        editToolBar.addAction(self.goToCellAction)

    def createStatusBar(self):
        self.locationLabel = QtWidgets.QLabel(" W999 ")
        self.locationLabel.setAlignment(QtCore.Qt.AlignHCenter)
        self.locationLabel.setMinimumSize(self.locationLabel.sizeHint())

        self.formulaLabel = QtWidgets.QLabel()
        self.formulaLabel.setIndent(3)

        self.statusBar().addWidget(self.locationLabel)
        self.statusBar().addWidget(self.formulaLabel, 1)

        self.spreadsheet.currentCellChanged.connect(self.updateStatusBar)
        self.spreadsheet.modified.connect(self.spreadsheetModified)

        self.updateStatusBar()

    def readSettings(self):
        settings = QtCore.QSettings("Software Inc.", "Spreadsheet")

        rect = settings.value("geometry", QtCore.QRect(200, 200, 400, 400))
        self.move(rect.topLeft())
        self.resize(rect.size())

        self.recentFiles = settings.value("recentFiles")
        self.updateRecentFileActions()

        showGrid = not (settings.value("showGrid", True) == "false")
        self.showGridAction.setChecked(showGrid)

        autoRecalc = not (settings.value("autoRecalc", True) == "false")
        self.autoRecalcAction.setChecked(autoRecalc)

    def writeSettings(self):
        settings = QtCore.QSettings("Software Inc.", "Spreadsheet")

        settings.setValue("geometry", self.geometry())
        settings.setValue("recentFiles", self.recentFiles)
        settings.setValue("showGrid", self.showGridAction.isChecked())
        settings.setValue("autoRecalc", self.autoRecalcAction.isChecked())

    def okToContinue(self):
        if self.isWindowModified():
            r = QtWidgets.QMessageBox.warning(self, self.tr("Spreadsheet"),
                                              self.tr("The document has been modified.\nDo you want to save your changes?"),
                                              QtWidgets.QMessageBox.Save | QtWidgets.QMessageBox.Discard | QtWidgets.QMessageBox.Cancel)

            if r == QtWidgets.QMessageBox.Save:
                return self.save()
            elif r == QtWidgets.QMessageBox.Cancel:
                return False
        return True

    def loadFile(self, fileName):
        if not self.spreadsheet.readFile(fileName):
            self.statusBar().showMessage(self.tr("Loading canceled"), 2000)
            return False

        self.setCurrentFile(fileName)
        self.statusBar().showMessage(self.tr("File loaded"), 2000)
        return True

    def saveFile(self, fileName):
        if not self.spreadsheet.writeFile(fileName):
            self.statusBar().showMessage(self.tr("Saving canceled"), 2000)
            return False

        self.setCurrentFile(fileName)
        self.statusBar().showMessage(self.tr("File saved"), 2000)
        return True

    def setCurrentFile(self, fileName):
        self.curFile = fileName
        self.setWindowModified(False)

        shownName = "Untitled"
        if self.curFile != "":
            shownName = self.strippedName(self.curFile)
            # self.recentFiles.remove(self.curFile)
            # self.recentFiles.prepend(self.curFile)
            self.updateRecentFileActions()

        self.setWindowTitle(self.tr("{0}[*] - {1}").format(shownName, self.tr("Spreadsheet")))

    def updateRecentFileActions(self):
        if self.recentFiles:
            for i in self.recentFiles:
                if not QtCore.QFile.exists(i):
                    i.remove()

            for j in range(0, self.MaxRecentFiles):
                if j < len(self.recentFiles):
                    text = self.tr("&%1 %2").arg(j + 1).arg(self.strippedName(self.recentFiles[j]))
                    self.recentFileActions[j].setText(text)
                    self.recentFileActions[j].setData(self.recentFiles[j])
                    self.recentFileActions[j].setVisible(True)
                else:
                    self.recentFileActions[j].setVisible(False)

        self.separatorAction.setVisible(not self.recentFiles)

    def strippedName(self, fullFileName):
        return QtCore.QFileInfo(fullFileName).fileName()


if __name__ == '__main__':

    import sys

    app = QtWidgets.QApplication(sys.argv)
    mainWin = MainWindow()
    mainWin.show()

    sys.exit(app.exec_())
