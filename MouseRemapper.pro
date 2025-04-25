QT += quick core gui

CONFIG += c++11
SOURCES += \
    main.cpp \
    hookmanager.cpp \
    configmanager.cpp \
    autostart.cpp

HEADERS += \
    hookmanager.h \
    configmanager.h \
    autostart.h

RESOURCES += qml.qrc

DESTDIR = bin