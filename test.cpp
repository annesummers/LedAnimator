/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "test.h"

#include "animationtests.h"
#include "ledtests.h"
#include "enginetests.h"
#include "ledanimcodectests.h"
#include "leddetailswidgetstests.h"
#include "frametests.h"

using namespace Test;
/*
#include <QStringList>
#include <QHash>
#include <QtTest/QtTest>

Test* Test::get() {
    static Test inst;
    return &inst;
}
*/
/*!
    \brief Run all unit tests


    The following arguments are recognized :
    <ul>
        <li>--restrict : only run tests to which an option is passed
        <li>@&lt;testname&gt;[:&lt;option&gt;] : pass an argument to a single test.
        The test will be run with --restrict on even if the option is empty
    </ul>
    All other arguments are forwarded to all unit tests. Arguments passed to test
    can be any of those supported by regular QtTest-based unit tests.
*/
/*int Test::runAll(int argc, char **argv) {
    //bool restrict = false;
    QStringList common;
    QHash<QString, QStringList> specific;
    for (int i = 0; i < argc; ++i) {
        QString s = QString::fromLocal8Bit(argv[i]);
        if (i && *argv[i] == '@') {
            int idx = s.indexOf(':');
            if (idx != -1)
                specific[s.mid(1, idx - 1)] << s.mid(idx + 1);
            else
                specific[s.mid(1)] = specific.value(s.mid(1));
        } else if (!qstrcmp(argv[i], "--restrict")) {
            restrict = true;
        } else {
            common << s;
        }
    }


    int ret = 0;
    foreach (QObject *o, m_testObjects) {
        //if (restrict &&
    if(!specific.contains(o->objectName()))
            continue;
        QStringList args = common;
        args << specific.value(o->objectName());
        if ((ret = QTest::qExec(o, args)))
            break;
    }
    return ret;
}

void Test::addTestObject(QObject *o) {
    m_testObjects << o;
}

Test::Test() {
}

Test::~Test() {
    qDeleteAll(m_testObjects);
}*/

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    AnimationTests tc1;
    LedTests tc2;
    EngineTests tc3;
    FrameTests tc4;
    LedAnimCodecTests tc5;
    LedDetailsWidgetsTests tc6;

    return QTest::qExec(&tc1, argc, argv) ||
           QTest::qExec(&tc2, argc, argv) ||
            QTest::qExec(&tc3, argc, argv) ||
            QTest::qExec(&tc4, argc, argv) ||
            QTest::qExec(&tc5, argc, argv) ||
            QTest::qExec(&tc6, argc, argv);
}
