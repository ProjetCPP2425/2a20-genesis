#-------------------------------------------------
# Project created by QtCreator 2018-10-26T21:45:23
#-------------------------------------------------

QT += core gui widgets sql serialport charts printsupport
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = app
TARGET = Atelier_Connexion

# Fichiers sources
SOURCES += \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    registerwindow.cpp

# Fichiers d'en-tête
HEADERS += \
    login.h \
    mainwindow.h \
    connection.h \
    registerwindow.h

# Fichiers d'interface
FORMS += \
    login.ui \
    mainwindow.ui

# Répertoires d'inclusion
INCLUDEPATH += include

# Règles de déploiement
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
