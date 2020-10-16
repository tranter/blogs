#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), count(0)
{
    ui->setupUi(this);

    timer = new QBasicTimer();
    statusBar()->showMessage(tr("Created timer %1").arg(timer->timerId()), 1000);

    connect(ui->quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::Clear);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::Start);
}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}

void MainWindow::Clear()
{
    count = 0;
    ui->lcdNumber->display(count / 10.0);
}

void MainWindow::Start()
{
    if (timer->isActive()) {
        timer->stop();
        ui->startButton->setText(tr("Start"));
        ui->timerTypeComboBox->setEnabled(true);
    } else {
        Qt::TimerType type = Qt::PreciseTimer;
        if (ui->timerTypeComboBox->currentText() == QLatin1String("Precise")) {
            type = Qt::PreciseTimer;
        } else if (ui->timerTypeComboBox->currentText() == QLatin1String("Coarse")) {
            type = Qt::CoarseTimer;
        } else if (ui->timerTypeComboBox->currentText() == QLatin1String("Very Coarse")) {
            type = Qt::VeryCoarseTimer;
        } else {
            Q_ASSERT(false);
        }
        timer->start(100, type, this);
        ui->startButton->setText(tr("Stop"));
        ui->timerTypeComboBox->setEnabled(false);
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    count++;
    ui->lcdNumber->display(count / 10.0);
    event->accept();
}
