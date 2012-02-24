#-------------------------------------------------
#
# Project created by QtCreator 2012-02-16T08:38:38
#
#-------------------------------------------------

QT       += core

QT       -= gui

# VC++2010
DEFINES += _CRT_SECURE_NO_WARNINGS

# GCC4
QMAKE_CXXFLAGS += -std=c++0x


TARGET = hunkdump
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    AnsiFile.cpp

HEADERS += \
    AnsiFile.h
