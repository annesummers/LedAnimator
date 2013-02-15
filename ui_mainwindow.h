/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri 15. Feb 10:53:32 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "animationdetailswidgets.h"
#include "ledgridwidget.h"
#include "playinfowidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    AnimatorUi::LedGridWidget *iLedGridWidget;
    AnimatorUi::PlayInfoWidget *iPlayInfoWidget;
    QWidget *widget_4;
    QWidget *widget_3;
    AnimatorUi::AnimationDetailsWidget *iAnimationDetailsWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(965, 390);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        iLedGridWidget = new AnimatorUi::LedGridWidget(centralWidget);
        iLedGridWidget->setObjectName(QString::fromUtf8("iLedGridWidget"));
        iLedGridWidget->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(iLedGridWidget, 0, 0, 2, 1);

        iPlayInfoWidget = new AnimatorUi::PlayInfoWidget(centralWidget);
        iPlayInfoWidget->setObjectName(QString::fromUtf8("iPlayInfoWidget"));

        gridLayout->addWidget(iPlayInfoWidget, 0, 1, 1, 1);

        widget_4 = new QWidget(centralWidget);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));

        gridLayout->addWidget(widget_4, 0, 2, 1, 1);

        widget_3 = new QWidget(centralWidget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));

        gridLayout->addWidget(widget_3, 1, 1, 1, 2);

        iAnimationDetailsWidget = new AnimatorUi::AnimationDetailsWidget(centralWidget);
        iAnimationDetailsWidget->setObjectName(QString::fromUtf8("iAnimationDetailsWidget"));

        gridLayout->addWidget(iAnimationDetailsWidget, 2, 0, 1, 3);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 965, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
