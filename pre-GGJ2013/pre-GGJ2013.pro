TARGET = pre-GGJ2013

CONFIG += qt debug

SOURCES += main.cpp \
    game.cpp

HEADERS += \
    game.hpp

QMAKE_CXXFLAGS += -std=c++11
