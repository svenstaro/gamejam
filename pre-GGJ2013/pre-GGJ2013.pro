TARGET = pre-GGJ2013

CONFIG += qt debug

SOURCES += main.cpp \
    game.cpp \
    gameobject.cpp \
    cutie.cpp

HEADERS += \
    game.hpp \
    gameobject.hpp \
    cutie.hpp

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    resources.qrc
