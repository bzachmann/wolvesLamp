TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    ../wolvesLamp/*.c* \
    ../wolvesLamp/*.ino \
    ../wolvesLamp/ledstrip.cpp

HEADERS += \
    ../wolvesLamp/*.h* \
    ../wolvesLamp/ledstrip.h

INCLUDEPATH += C:/Program Files (x86)/Arduino/hardware/arduino/avr/cores/arduino

