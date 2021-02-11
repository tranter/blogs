#include "rectanglemanager.h"

#include <QTimer>

RectangleManager::RectangleManager(QObject *parent) : QObject(parent)
{
    QTimer * timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    connect(timer, &QTimer::timeout, this, [&](){
        static int i = 0;
        b_rectangleCount = 10 + i;

        // Before Qt 6 Equivalent action:
//        setRectangleCount(10+i);

        i++;
    });
}

int RectangleManager::rectangleCount() const
{
    // NOTE:
    // The following lines can the same if the
    // private member variable name is the same.
    // For the purpose of demonstrating bindings,
    // the variable name is slightly changed

    // Before Qt 6
//    return m_rectangleCount;

    // Qt 6 +
    return b_rectangleCount;
}

void RectangleManager::setRectangleCount(int rectangleCount)
{
    // Before Qt 6:
//    if (m_rectangleCount == rectangleCount)
//        return;

//    m_rectangleCount = rectangleCount;
//    emit rectangleCountChanged();


    // Qt 6+ :
    b_rectangleCount = rectangleCount;
}
