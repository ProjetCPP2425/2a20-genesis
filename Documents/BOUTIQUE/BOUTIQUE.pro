#-------------------------------------------------
#
# Project created by QtCreator 2018-10-26T21:45:23
#
#-------------------------------------------------

QT += core gui widgets charts sql texttospeech quick quickwidgets quickcontrols2
# Pour les warnings de dépréciation
QMAKE_CXXFLAGS += -Wno-deprecated-declarations
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += serialport sql

TARGET = BOUTIQUE[master]
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    Connection.cpp \
    arduino.cpp \
    boutique.cpp \
    main.cpp \
    mainwindow.cpp \
    statistique.cpp

HEADERS += \
    Connection.h \
    arduino.h \
    boutique.h \
    mainwindow.h \
    statistique.h

FORMS += \
    mainwindow.ui \
    statistique.ui

RESOURCES +=

DISTFILES += \
    marker.png

