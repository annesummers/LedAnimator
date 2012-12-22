/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include <QApplication>
#include "mainwindow.h"
#include "engine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Engine engine;
    engine.start();
    
    return a.exec();
}
