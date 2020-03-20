#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->moveToTrashButton->setEnabled(false);
    ui->restoreButton->setEnabled(false);
    
    connect(ui->selectFileButton, &QAbstractButton::clicked, this, &MainWindow::selectFile);
    connect(ui->moveToTrashButton, &QAbstractButton::clicked, this, &MainWindow::moveToTrash);
    connect(ui->restoreButton, &QAbstractButton::clicked, this, &MainWindow::restore);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select File"));
    m_filename = filename;
    ui->filename->setText(filename);
    ui->trashpath->clear();
    ui->moveToTrashButton->setEnabled(true);
    ui->restoreButton->setEnabled(false);
}

void MainWindow::moveToTrash()
{
    QString pathInTrash;

    if (ui->filename->text().isEmpty()) {
        return;
    }

    if (QMessageBox::question(this,
                              tr("Confirmation"),
                              tr("Are you sure you want to move\n%1\nto the trash?").arg(ui->filename->text()))
        == QMessageBox::Yes) {
        if (QFile::moveToTrash(ui->filename->text(), &pathInTrash)) {
            ui->filename->clear();
            ui->trashpath->setText(pathInTrash);
            ui->moveToTrashButton->setEnabled(false);
            ui->restoreButton->setEnabled(true);
        } else {
            QMessageBox::warning(this, tr("Warning"), tr("Move to trash failed."));
        }
    }
}

void MainWindow::restore()
{
    if (ui->trashpath->text().isEmpty() || m_filename.isEmpty()) {
        return;
    }

    if (QFile::rename(ui->trashpath->text(), m_filename)) {
        ui->filename->setText(m_filename);
        ui->trashpath->clear();
        ui->moveToTrashButton->setEnabled(true);
        ui->restoreButton->setEnabled(false);
    } else {
        QMessageBox::warning(this, tr("Warning"), tr("File restore failed."));
    }
}
