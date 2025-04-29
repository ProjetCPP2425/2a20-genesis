#-------------------------------------------------
#
# Project created by QtCreator 2018-10-26T21:45:23
#
#-------------------------------------------------

QT += core gui widgets charts printsupport svg sql
QT += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += serialport sql

TARGET = BOUTIQUE[master]
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    Connection.cpp \
    arduino.cpp \
    camembertdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    qrcode.cpp \
    ressource.cpp \
    qrcodegen.cpp

HEADERS += \
    Connection.h \
    arduino.h \
    camembertdialog.h \
    mainwindow.h \
    qrcode.h \
    ressource.h \
    qrcodegen.hpp

FORMS += \
    mainwindow.ui

INCLUDEPATH += .

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
