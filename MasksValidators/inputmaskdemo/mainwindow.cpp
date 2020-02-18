#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect buttons to slots.
    connect(ui->quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clear);
    connect(ui->helpButton, &QPushButton::clicked, this, &MainWindow::help);

    // Update input mask when it changes. Only need to do one thing, so we use a lambda function.
    connect(ui->maskLineEdit, &QLineEdit::textChanged, [=](const QString &newMask) { ui->inputLineEdit->setInputMask(newMask); });

    // Update status bar when line edit changes.
    connect(ui->inputLineEdit, &QLineEdit::textChanged, this, &MainWindow::updateStatusBar);

    // Update status bar when return pressed.
    connect(ui->inputLineEdit, &QLineEdit::returnPressed, [=]() { this->statusBar()->showMessage(tr("Return pressed")); });

    // Update status bar when editing finished.
    connect(ui->inputLineEdit, &QLineEdit::editingFinished, [=]() { this->statusBar()->showMessage(tr("Editing finished")); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear()
{
    ui->inputLineEdit->clear();
    ui->maskLineEdit->clear();
}

void MainWindow::updateStatusBar()
{
    if (ui->inputLineEdit->hasAcceptableInput()) {
        this->statusBar()->showMessage(tr("Input acceptable"));
    } else {
        this->statusBar()->showMessage(tr("Input not acceptable"));
    }
}

void MainWindow::help()
{
    QString helpString =
        tr("<b>Help on Mask String</b><br>"
           "<br>"
           "The mask consists of a string of mask characters and separators, "
           "optionally followed by a semicolon and the character used for blanks. "
           "The blank characters are always removed from the text after editing. "
           "A space character, the default character for a blank, is needed for "
           "cases where a character is permitted but not required. "
           "The following characters that can be used in an input mask:<br>"
           "<br>"
           "A - ASCII alphabetic character required. A-Z, a-z.<br>"
           "a - ASCII alphabetic character permitted but not required.<br>"
           "N - ASCII alphanumeric character required. A-Z, a-z, 0-9.<br>"
           "n - ASCII alphanumeric character permitted but not required.<br>"
           "X - Any character required.<br>"
           "x - Any character permitted but not required.<br>"
           "9 - ASCII digit required. 0-9.<br>"
           "0 - ASCII digit permitted but not required.<br>"
           "D - ASCII digit required. 1-9.<br>"
           "d - ASCII digit permitted but not required (1-9).<br>"
           "# - ASCII digit or plus/minus sign permitted but not required.<br>"
           "H - Hexadecimal character required. A-F, a-f, 0-9.<br>"
           "h - Hexadecimal character permitted but not required.<br>"
           "B - Binary character required. 0-1.<br>"
           "b - Binary character permitted but not required.<br>"
           "> - All following alphabetic characters are uppercased.<br>"
           "< - All following alphabetic characters are lowercased.<br>"
           "! - Switch off case conversion.<br>"
           "[ ] { } - Reserved.<br>"
           "<br>"
           "Use \\ to escape the special characters listed above to use them as separators.");

        QMessageBox::information(this, tr("Help"), helpString);
}
