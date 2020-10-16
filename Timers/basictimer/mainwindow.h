#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBasicTimer>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void Clear();
    void Start();

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    Ui::MainWindow *ui;
    QBasicTimer *timer;
    long count;
};

#endif // MAINWINDOW_H
