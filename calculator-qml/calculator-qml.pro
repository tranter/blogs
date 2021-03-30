TARGET = calculator
TEMPLATE = app

QT += qml quick

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build

SOURCES += \
    main.cpp\
    calculator.cpp

HEADERS  += \
    calculator.h

OTHER_FILES += \
    qml/Button.qml \
    qml/Calculator.qml

RESOURCES += \
    qml/qml.qrc

