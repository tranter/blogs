TARGET   = multilangtest
TEMPLATE = app
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG   += c++11

lessThan(QT_MAJOR_VERSION, 5): unix: QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    translation.cpp \
    dialog.cpp \
    TranslatorManager.cpp

HEADERS += \
    mainwindow.h \
    translation.h \
    dialog.h \
    TranslatorManager.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    multiLangTest.qrc
