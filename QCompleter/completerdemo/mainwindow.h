#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCompleter>
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
    void UpdateSettings();
    void Highlighted(const QString &text);
    void Activated(const QString &text);

private:
    Ui::MainWindow *ui;
    QCompleter *completer;
};

#endif // MAINWINDOW_H
