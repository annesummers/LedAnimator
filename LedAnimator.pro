#-------------------------------------------------
#
# Project created by QtCreator 2012-12-01T17:01:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LedAnimator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    led.cpp \
    ledwidget.cpp \
    engine.cpp \
    animation.cpp \
    frame.cpp \
    ledgridwidget.cpp \
    playinfowidget.cpp \
    framedetailswidget.cpp \
    ledanimcodec.cpp

HEADERS  += mainwindow.h \
    led.h \
    ledwidget.h \
    engine.h \
    animation.h \
    frame.h \
    ledgridwidget.h \
    playinfowidget.h \
    framedetailswidget.h \
    defaults.h \
    ledanimcodec.h \
    exceptions.h

FORMS    +=

OTHER_FILES += \
    .gitignore
