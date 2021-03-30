#include <QDebug>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>

#include "calculator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // create view window
    QQuickView *view = new QQuickView;
    view->setSource(QUrl("qrc:/Calculator.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->resize(300, 400);

    // create Calculator object
    Calculator calculator;
    calculator.setRootObject(view->rootObject());

    QObject::connect(view->engine(), SIGNAL(quit()), view, SLOT(close()));
    view->show();

    return app.exec();
}
