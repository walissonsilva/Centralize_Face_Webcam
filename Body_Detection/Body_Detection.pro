#-------------------------------------------------
#
# Project created by QtCreator 2015-12-22T08:49:15
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Body_Detection
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config opencv --libs`

SOURCES += main.cpp
