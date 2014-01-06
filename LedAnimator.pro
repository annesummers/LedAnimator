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

    include(Test.pri)

} else {
    TARGET = LedAnimator
    TEMPLATE = app

    SOURCES += \
        main.cpp

    FORMS    +=

    OTHER_FILES += \
        .gitignore
}

include(Model.pri)
include(UI.pri)

HEADERS += \
    gammacorrect.h \
    constants.h \
    exceptions.h \
    AnimationDetails.h \
    AnimatorApplication.h \
    Engine.h

SOURCES += \
    AnimationDetails.cpp \
    AnimatorApplication.cpp \
    Engine.cpp

RESOURCES += \
    LedAnimator.qrc

OTHER_FILES += \
    README.txt \
    .gitignore \
    LICENSE.txt \
    Model.pri \
    Test.pri \
    UI.pri


