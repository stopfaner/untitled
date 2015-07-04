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
    Sources/UserInterface/uielement.cpp \
    Sources/UserInterface/HUD/hudelement.cpp \
    Sources/UserInterface/Screen/scene.cpp \
    Sources/UserInterface/Screen/camera.cpp \
    Sources/GameObjects/entity.cpp \
    Sources/GameObjects/actor.cpp \
    Sources/GameObjects/Characters/player.cpp \
    Sources/GameObjects/Characters/bot.cpp \
    Sources/Process/gamemod.cpp

HEADERS  += \
    Sources/gamewidget.h \
    Sources/gameinstance.h \
    Sources/GameStates/gamestate.h \
    Sources/UserInterface/uielement.h \
    Sources/UserInterface/HUD/hudelement.h \
    Sources/UserInterface/Screen/scene.h \
    Sources/UserInterface/Screen/camera.h \
    Sources/GameObjects/entity.h \
    Sources/GameObjects/actor.h \
    Sources/GameObjects/Characters/player.h \
    Sources/GameObjects/Characters/bot.h \
    Sources/Process/gamemod.h
