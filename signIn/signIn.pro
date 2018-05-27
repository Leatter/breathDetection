#-------------------------------------------------
#
# Project created by QtCreator 2018-01-17T21:20:12
#
#-------------------------------------------------

QT       += core gui sql serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = signIn
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    register.cpp \
    forgetpassword.cpp \
    mysqldatabase.cpp \
    resetuppassword.cpp \
    operation.cpp \
    qcustomplot.cpp \
    serialportconf.cpp \
    SubThread.cpp

HEADERS  += widget.h \
    register.h \
    forgetpassword.h \
    mysqldatabase.h \
    resetuppassword.h \
    operation.h \
    qcustomplot.h \
    serialportconf.h \
    SubThread.h

FORMS    += widget.ui \
    register.ui \
    forgetpassword.ui \
    resetuppassword.ui \
    operation.ui \
    serialportconf.ui

RESOURCES += \
    image.qrc
