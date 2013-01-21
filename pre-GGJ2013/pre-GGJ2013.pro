TARGET = pre-GGJ2013

CONFIG += qt debug

QT += opengl

SOURCES += main.cpp \
    game.cpp \
    gameobject.cpp \
    cutie.cpp \
    sprite.cpp \
    arena.cpp

HEADERS += \
    game.hpp \
    gameobject.hpp \
    cutie.hpp \
    sprite.hpp \
    arena.hpp

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    resources.qrc
