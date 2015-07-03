#-------------------------------------------------
#
# Project created by QtCreator 2015-07-02T22:51:41
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    Sources/gamewidget.cpp \
    Sources/GameStates/gamestate.cpp \
    Sources/GameObjects/gameobject.cpp \
    Sources/UserInterface/uielement.cpp

HEADERS  += \
    Sources/gamewidget.h \
    Sources/gameinstance.h \
    Sources/GameStates/gamestate.h \
    Sources/GameObjects/gameobject.h \
    Sources/UserInterface/uielement.h
