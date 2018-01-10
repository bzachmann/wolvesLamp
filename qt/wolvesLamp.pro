TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    ../wolvesLamp/*.c* \
    ../wolvesLamp/*.ino

HEADERS += \
    ../wolvesLamp/*.h*

INCLUDEPATH += C:/Program Files (x86)/Arduino/hardware/arduino/avr/cores/arduino

