/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef ANIMATORAPPLICATION_H
#define ANIMATORAPPLICATION_H

#include <QApplication>

class AnimatorApplication : public QApplication
{
public:
    AnimatorApplication(int argc, char *argv[]);

protected:
    bool notify(QObject *rec, QEvent *ev);
};

#endif // ANIMATORAPPLICATION_H
