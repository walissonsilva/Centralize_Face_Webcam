#-------------------------------------------------
#
# Project created by QtCreator 2015-10-25T19:47:11
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Centralize_Face
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config opencv --libs`

SOURCES += main.cpp \
    rs232.cpp

HEADERS += \
    rs232.h
