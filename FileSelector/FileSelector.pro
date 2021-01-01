lessThan(QT_MAJOR_VERSION, 5): error(This project requires Qt 5 or later)

TEMPLATE = app
QT += qml quick
CONFIG += c++11
SOURCES += main.cpp
RESOURCES += qml.qrc
