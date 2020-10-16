#include <QMessageBox>
#include <QElapsedTimer>
#include <QDebug>
#include <cmath>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(ui->infoButton, &QPushButton::clicked, this, &MainWindow::Info);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::Start);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Info()
{
    QString msg;

    msg += "Clock Type: ";
    switch (QElapsedTimer::clockType()) {
    case QElapsedTimer::SystemTime:
        msg += "System Time";
        break;
    case QElapsedTimer::MonotonicClock:
        msg += "Monotonic Clock";
        break;
    case QElapsedTimer::TickCounter:
        msg += "Tick Counter";
        break;
    case QElapsedTimer::MachAbsoluteTime:
        msg += "Mach Absolute Time";
        break;
    case QElapsedTimer::PerformanceCounter:
        msg += "Performance Counter";
        break;
    }

    msg += "\n";

    if (QElapsedTimer::isMonotonic()) {
        msg += "Is Monotonic: Yes";
    } else {
       msg += "Is Monotonic: No";
    }

    QMessageBox::information(this, tr("Timer Info"), msg);
}

void MainWindow::Start()
{
    long repetitions = ui->repetitionSpinBox->value();
    double result = 0.0;

    QElapsedTimer timer;
    timer.start();

    if (ui->operationComboBox->currentText() == "Multiply") {
        for (long i = 0; i < repetitions; i++) {
            result = double(rand()) * double(rand());
            // Example of how to check for timer expiring. Give up if 1 second has elapsed.
            if (timer.hasExpired(1000)) {
                break;
            }
        }
    } else if (ui->operationComboBox->currentText() == "Divide") {
        for (long i = 0; i < repetitions; i++) {
            result = double(rand()) / double(rand());
        }
    } else if (ui->operationComboBox->currentText() == "Sine") {
        for (long i = 0; i < repetitions; i++) {
            result = sin(double((rand())));
        }
    } else if (ui->operationComboBox->currentText() == "Cosine") {
        for (long i = 0; i < repetitions; i++) {
            result = cos(double((rand())));
        }
    } else if (ui->operationComboBox->currentText() == "Nothing") {
        for (long i = 0; i < repetitions; i++) {
            ;
        }
    } else {
        Q_ASSERT(false);
    }

    qint64 elapsedNsec = timer.nsecsElapsed();
    qint64 elapsedMsec = timer.elapsed();
    QString msg;
    msg += tr("The operation took %1 nanoseconds\n").arg(elapsedNsec);
    msg += tr("or %1 milliseconds.\n").arg(elapsedMsec);
    msg += tr("Time since reference: %1 milliseconds.\n").arg(timer.msecsSinceReference());
    msg += "Has 100 msec expired? ";
    timer.hasExpired(100) ? msg += "yes" : msg += "no";
    msg += ".\n";
    msg += "Timer valid? ";
    timer.isValid() ? msg += "yes" : msg += "no";
    msg += ".\n";
    msg += tr("Result was %1.").arg(result);

    QMessageBox::information(this, tr("Elapsed Time"), msg);
    timer.invalidate();
}
