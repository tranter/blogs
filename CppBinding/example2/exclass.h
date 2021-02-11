#ifndef EXCLASS_H
#define EXCLASS_H

#include <QObject>
class ExClass : public QObject{
    Q_OBJECT
public:
    QProperty<QString> stringProperty; // for Qt 6+

    explicit ExClass(QObject *parent = nullptr);

    void setStringText(QString str);
    QString getString();
signals:
    void stringChanged(QString string);
private:
    QString m_string;
};

#endif // EXCLASS_H
