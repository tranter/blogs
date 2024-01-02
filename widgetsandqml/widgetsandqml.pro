lessThan(QT_MAJOR_VERSION, 5): error(This project requires Qt 5 or later)

QT += core gui widgets quick

TARGET = widgetsandqml
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

OTHER_FILES += main.qml
