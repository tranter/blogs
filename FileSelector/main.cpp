#include <QDebug>
#include <QFileSelector>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Create and display a file selector, just for interest.
    QFileSelector sel;
    qDebug() << sel.allSelectors();

    // Load and execute the main QML file.
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
