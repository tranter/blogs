#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCalendar>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setSystem(int);
    void update();

private:
    Ui::MainWindow *ui;
    QCalendar *calendar;
};
#endif // MAINWINDOW_H
