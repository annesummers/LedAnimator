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
        frametests.cpp \
        ledanimcodectests.cpp \
        colourwidgettests.cpp

    HEADERS += \
        animationtests.h\
        ledtests.h \
        enginetests.h \
        frametests.h \
        ledanimcodectests.h \
        colourwidgettests.h

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
   # leddetailswidgets.h \
    animationdetailswidgets.h \
    animatorapplication.h \
    selectable.h \
    ledwidget.h \
    framewidget.h \
    framelistwidget.h \
    fadecalculator.h \
    gammacorrect.h \
    colourgroupwidget.h \
    colourgroupgroupwidget.h \
    colourwidget.h \
    newanimationdialog.h

SOURCES += \
    animatorbase.cpp \
    mainwindow.cpp \
    led.cpp \
    engine.cpp \
    animation.cpp \
    frame.cpp \
    ledgridwidget.cpp \
    playinfowidget.cpp \
    ledanimcodec.cpp \
   # leddetailswidgets.cpp \
    animationdetailswidgets.cpp \
    animatorapplication.cpp \
    selectable.cpp \
    ledwidget.cpp \
    framewidget.cpp \
    framelistwidget.cpp \
    fadecalculator.cpp \
    colourgroupwidget.cpp \
    colourwidget.cpp \
    colourgroupgroupwidget.cpp \
    newanimationdialog.cpp

FORMS += \
    newanimation.ui \
    mainwindow.ui \
    animationdetailswidget.ui \
    playinfowidget.ui


