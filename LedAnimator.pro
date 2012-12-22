#-------------------------------------------------
#
# Project created by QtCreator 2012-12-01T17:01:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(testing) {
    TARGET = LedAnimatorTests # different app name for testing suite
    QT += testlib
    CONFIG += qtestlib
    TEMPLATE = app

    SOURCES += \
        test.cpp \
        animationtests.cpp\
        ledtests.cpp \
        enginetests.cpp \
        ledgridwidgettests.cpp \
        ledwidgettests.cpp \
        playinfowidgettests.cpp \
        frametests.cpp \
        framewidgettests.cpp \
        ledanimcodectests.cpp \
        animationdetailswidgettest.cpp \
        leddetailswidgetstests.cpp

    HEADERS += \
        test.h \
        animationtests.h\
        ledtests.h \
        enginetests.h \
        ledgridwidgettests.h \
        ledwidgettests.h \
        playinfowidgettests.h \
        frametests.h \
        framewidgettests.h \
        ledanimcodectests.h \
        animationdetailswidgettest.h \
        leddetailswidgetstests.h

} else {
    TARGET = LedAnimator
    TEMPLATE = app

    SOURCES += \
        main.cpp

    FORMS    +=

    OTHER_FILES += \
        .gitignore
}

HEADERS += \
    animatorbase.h \
    animatorwidgetbase.h \
    mainwindow.h \
    led.h \
    engine.h \
    animation.h \
    frame.h \
    ledgridwidget.h \
    playinfowidget.h \
    defaults.h \
    ledanimcodec.h \
    exceptions.h \
    framewidgets.h \
    leddetailswidgets.h \
    ledwidgets.h \
    animationdetailswidgets.h

SOURCES += \
    animatorbase.cpp \
    animatorwidgetbase.cpp\
    mainwindow.cpp \
    led.cpp \
    engine.cpp \
    animation.cpp \
    frame.cpp \
    ledgridwidget.cpp \
    playinfowidget.cpp \
    ledanimcodec.cpp \
    framewidgets.cpp \
    leddetailswidgets.cpp \
    ledwidgets.cpp \
    animationdetailswidgets.cpp


