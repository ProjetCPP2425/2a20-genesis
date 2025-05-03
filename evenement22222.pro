QT       += core gui
QT += printsupport
QT += charts

QT += core gui charts

QT       += core gui  serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    connection.cpp \
    evenements.cpp \
    historydialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    connection.h \
    evenements.h \
    historydialog.h \
    mainwindow.h

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
