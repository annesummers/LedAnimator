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
        colourwidgettests.cpp \
        selectablewidgettests.cpp \
        selectablewidgettestbase.cpp \
        ledsocketinteractiontests.cpp

    HEADERS += \
        animationtests.h\
        ledtests.h \
        enginetests.h \
        frametests.h \
        ledanimcodectests.h \
        colourwidgettests.h \
        selectablewidgettests.h \
        selectablewidgettestbase.h \
        ledsocketinteractiontests.h \
        testconstants.h

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
    mainwindow.h \
    led.h \
    engine.h \
    animation.h \
    frame.h \
    ledgridwidget.h \
    playinfowidget.h \
    constants.h \
    ledanimcodec.h \
    exceptions.h \
    animationdetailswidget.h \
    animatorapplication.h \
    selectable.h \
    ledwidget.h \
    framewidget.h \
    framelistwidget.h \
    fadecalculator.h \
    gammacorrect.h \
    colourgroupwidget.h \
    colourwidget.h \
    newanimationdialog.h \
    socketwidget.h \
    selectablegroupwidget.h \
    selectablewidget.h \
    griditem.h \
    animationdetailswidget.h \
    selectablegroupgroupwidget.h \
    grouplistwidget.h \
    colourgroupgroupwidget.h \
    ledanimatoractionbase.h \
    leddetails.h \
    framedetails.h \
    renumberleddialog.h \
    ledgridgroupwidget.h

SOURCES += \
    mainwindow.cpp \
    led.cpp \
    engine.cpp \
    animation.cpp \
    frame.cpp \
    ledgridwidget.cpp \
    playinfowidget.cpp \
    ledanimcodec.cpp \
    animatorapplication.cpp \
    selectable.cpp \
    ledwidget.cpp \
    framewidget.cpp \
    framelistwidget.cpp \
    fadecalculator.cpp \
    colourgroupwidget.cpp \
    colourwidget.cpp \
    newanimationdialog.cpp \
    socketwidget.cpp \
    selectablegroupwidget.cpp \
    selectablewidget.cpp \
    griditem.cpp \
    animationdetailswidget.cpp \
    selectablegroupgroupwidget.cpp \
    grouplistwidget.cpp \
    colourgroupgroupwidget.cpp \
    ledanimatoractionbase.cpp \
    leddetails.cpp \
    framedetails.cpp \
    renumberleddialog.cpp \
    ledgridgroupwidget.cpp

FORMS += \
    newanimation.ui \
    gridlistwidget.ui \
    leddetails.ui \
    framedetails.ui \
    renumberleddialog.ui

RESOURCES += \
    LedAnimator.qrc

OTHER_FILES += \
    README.txt \
    .gitignore \
    LICENSE.txt


