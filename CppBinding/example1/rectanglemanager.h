#ifndef RECTANGLEMANAGER_H
#define RECTANGLEMANAGER_H

#include <QObject>

class RectangleManager : public QObject
{
    Q_OBJECT
    // Before Qt 6 : The Q_PROPERTY without binding:
//    Q_PROPERTY(int rectangleCount READ rectangleCount
//                         WRITE setRectangleCount
//                         NOTIFY rectangleCountChanged
//               )

    // Qt 6+ :  The Q_PROPERTY with binding
    Q_PROPERTY(int rectangleCount READ rectangleCount
                         WRITE setRectangleCount
                         BINDABLE bindableRectangleCount
     )

public:
    explicit RectangleManager(QObject *parent = nullptr);
    int rectangleCount() const;

    // Qt 6+ : Function passed to Q_PROPERTY for the BINDABLE macro
    QBindable<int> bindableRectangleCount() { return &b_rectangleCount; }

public slots:
    void setRectangleCount(int rectangleCount);

signals:
    void rectangleCountChanged();

private:
    // Before Qt 6:
//    int m_rectangleCount;


    // Qt 6+ :
        Q_OBJECT_BINDABLE_PROPERTY(RectangleManager,
                               int,
                               b_rectangleCount,
                               &RectangleManager::rectangleCountChanged)
};

#endif // RECTANGLEMANAGER_H
