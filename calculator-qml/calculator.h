#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>

class Calculator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString display READ display WRITE setDisplay NOTIFY displayChanged)

public:
    Calculator(QObject *parent = 0);
    ~Calculator();

    void setRootObject(QObject *root);

    QString display() const { return mDisplay; }

public slots:
    void setDisplay(const QString &display);

    void keyClicked(int key);
    void editCut();
    void editCopy();
    void editPaste();

signals:
    void displayChanged();

private:
    void clearAll();
    double calculate(double rightoperand, int operation);

private slots:
    void onDisplayChanged();

private:
    QObject *mRoot;
    QString mDisplay;
    double mResult;
    bool mExpectingOperand;
    int mPendingOperator;
};

#endif // CALCULATOR_H
