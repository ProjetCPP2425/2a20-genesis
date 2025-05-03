QT       += core gui
QT += printsupport
QT += charts

QT += core gui charts

QT       += core gui  serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    arduino.cpp \
    camembertdialog.cpp \
    connection.cpp \
    evenements.cpp \
    historydialog.cpp \
    main.cpp \
    mainwindow.cpp \
    qrcode.cpp \
    qrcodegen.cpp \
    ressource.cpp

HEADERS += \
    arduino.h \
    camembertdialog.h \
    connection.h \
    evenements.h \
    historydialog.h \
    mainwindow.h \
    qrcode.h \
    qrcodegen.hpp \
    ressource.h

FORMS += \
    mainwindow.ui

QT+= sql
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    bell.qrc \
    icons.qrc \
    logo.qrc
