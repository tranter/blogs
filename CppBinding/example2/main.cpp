#include "exclass.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ExClass *object1 = new ExClass();
    ExClass *relatedObject2 = new ExClass();

    QObject::connect(object1, &ExClass::stringChanged,
                     relatedObject2, &ExClass::setStringText);

    // for < Qt 6.0
    qDebug()<< "relatedObject2 value before object1 sets value: " << relatedObject2->getString();

    object1->setStringText("new object1 Hi");
    qDebug()<< "relatedObject2 value after object1 sets value: " << relatedObject2->getString();

    // for >= Qt 6.0
    qDebug()<< "\nBEYOND Qt 6.0";
    object1->stringProperty = "object1 Hi";
    qDebug()<< "relatedObject2 value before binding: " << relatedObject2->stringProperty;

    relatedObject2->stringProperty.setBinding([&](){return object1->stringProperty.value();});
    qDebug()<< "relatedObject2 value after binding: " << relatedObject2->stringProperty;

    object1->stringProperty = "New object1 Hi";
    qDebug()<< "relatedObject2 value after object1's value changed: " << relatedObject2->stringProperty;

    return a.exec();
}
