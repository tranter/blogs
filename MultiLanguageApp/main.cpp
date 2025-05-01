#include <QApplication>
#include "mainwindow.h"
#include "TranslatorManager.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(multiLangTest);

    QApplication a(argc, argv);

    // Translator must be created before the application's widgets.
    TranslatorManager* manager = TranslatorManager::instance();
    Q_UNUSED(manager)

    MainWindow w;
    w.show();

    return a.exec();
}
