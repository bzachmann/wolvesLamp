TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    ../wolvesLamp/*.c* \
    ../wolvesLamp/*.ino \
    ../wolvesLamp/twitchstreamerinfo.cpp \
    ../wolvesLamp/marqueestring.cpp \
    ../wolvesLamp/twitchinfodisplay.cpp

HEADERS += \
    ../wolvesLamp/*.h* \
    ../wolvesLamp/twitchstreamerinfo.h \
    ../wolvesLamp/marqueestring.h \
    ../wolvesLamp/twitchinfodisplay.h

INCLUDEPATH += C:/Program Files (x86)/Arduino/hardware/arduino/avr/cores/arduino

