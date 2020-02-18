#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QRegularExpressionValidator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setValidator();
    void updateStatusBar();
    void checkRegularExpression();

    Ui::MainWindow *ui;
    QIntValidator *intValidator;
    QDoubleValidator *doubleValidator;
    QRegularExpressionValidator *regularExpressionValidator;
};

#endif // MAINWINDOW_H
