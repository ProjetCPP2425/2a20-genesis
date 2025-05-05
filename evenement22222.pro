QT       += core gui
QT += printsupport
QT += charts

QT += core gui charts

QT       += core gui  serialport
QT +=texttospeech quick quickwidgets quickcontrols2
QT+=network

QT += core gui widgets sql serialport charts printsupport
DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    arduino.cpp \
    boutique.cpp \
    camembertdialog.cpp \
    connection.cpp \
    evenements.cpp \
    historydialog.cpp \
    locataire.cpp \
    login.cpp \
    mailing.cpp \
    main.cpp \
    mainwindow.cpp \
    qrcode.cpp \
    qrcodegen.cpp \
    registerwindow.cpp \
    ressource.cpp \
    statistique.cpp

HEADERS += \
    arduino.h \
    boutique.h \
    camembertdialog.h \
    connection.h \
    evenements.h \
    historydialog.h \
    locataire.h \
    login.h \
    mailing.h \
    mainwindow.h \
    qrcode.h \
    qrcodegen.hpp \
    registerwindow.h \
    ressource.h \
    statistique.h

FORMS += \
    login.ui \
    mainwindow.ui \
    statistique.ui

QT+= sql
QT+= serialport
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    bell.qrc \
    icons.qrc \
    logo.qrc

DISTFILES += \
    eye.png
