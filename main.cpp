/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "animatorapplication.h"
#include "mainwindow.h"
#include "engine.h"

int main(int argc, char *argv[])
{
    AnimatorApplication a(argc, argv);

    Engine engine;
    engine.start();
    
    return a.exec();
}
