/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animatorapplication.h"

#include <QMessageBox>

#include "exceptions.h"

using namespace Exception;

AnimatorApplication::AnimatorApplication(int argc, char *argv[]) :
    QApplication(argc, argv)
{
}

bool AnimatorApplication::notify(QObject *rec, QEvent *ev) {
    try {
        return QApplication::notify(rec, ev);

    } catch(AnimatorException& e) {
        qDebug() << "Exception: " << e.errorMessage();

        QMessageBox::critical(NULL, "Exception", QString("LedAnimator has thrown an exception:\n").append(e.errorMessage()));

        exit(0);
    }

    return false;
}
