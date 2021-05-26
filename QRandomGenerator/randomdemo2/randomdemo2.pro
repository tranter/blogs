QT       += core gui

lessThan(QT_MAJOR_VERSION, 5) | lessThan(QT_MINOR_VERSION, 10): error(This project requires Qt 5.10 or later)
  
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = randomdemo2
TEMPLATE = app

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui
