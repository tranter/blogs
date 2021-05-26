#include <stdlib.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QPainter>
#include <QRandomGenerator>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Refresh every second.
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QElapsedTimer timer;
    //QRandomGenerator gen = QRandomGenerator();

    timer.start();

#if 1
    // Plot random colors
    for (int x = 0; x < this->width(); ++x) {
        for (int y = 0; y < this->height(); ++y) {

            // Code using rand()
            //QColor color = QColor(rand() % 256, rand() % 256, rand() % 256);

            // Code using qrand()
            //QColor color = QColor(qrand() % 256, qrand() % 256, qrand() % 256);
            
            // Code using default QRandomGenerator() with a seed, so data is the same every iteration.
            //QColor color = QColor(gen.bounded(256), gen.bounded(256), gen.bounded(256));

            // Code using system QRandomGenerator()
            //QColor color = QColor(QRandomGenerator::system()->bounded(256), QRandomGenerator::system()->bounded(256), QRandomGenerator::system()->bounded(256));

            // Code using global QRandomGenerator()
            QColor color = QColor(QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256), QRandomGenerator::global()->bounded(256));

            painter.setPen(color);
            painter.drawPoint(x, y);
        }
    }
#endif

#if 0
    // Alternate code - plot random pixels
    for (int i = 0; i < (this->width() * this->height()) / 2; i++) {

        int x = qrand() % this-> width();
        int y = qrand() % this-> height();

        painter.setPen(Qt::black);
        painter.drawPoint(x, y);

    }
#endif

    qDebug() << "Paint event took" << timer.elapsed() << "milliseconds.";
}
