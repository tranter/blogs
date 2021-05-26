TEMPLATE = app
TARGET = randomdemo1

lessThan(QT_MAJOR_VERSION, 5) | lessThan(QT_MINOR_VERSION, 10): error(This project requires Qt 5.10 or later)

# Input
SOURCES += randomdemo1.cpp
