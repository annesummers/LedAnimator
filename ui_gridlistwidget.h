/********************************************************************************
** Form generated from reading UI file 'gridlistwidget.ui'
**
** Created: Fri 1. Mar 14:52:58 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDLISTWIDGET_H
#define UI_GRIDLISTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GridListWidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *widget;

    void setupUi(QWidget *GridListWidget)
    {
        if (GridListWidget->objectName().isEmpty())
            GridListWidget->setObjectName(QString::fromUtf8("GridListWidget"));
        GridListWidget->resize(352, 271);
        horizontalLayout_2 = new QHBoxLayout(GridListWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(GridListWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(groupBox);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        gridLayout->addWidget(tableWidget, 0, 0, 1, 3);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(31, 32));
        pushButton->setMaximumSize(QSize(31, 32));

        gridLayout->addWidget(pushButton, 1, 0, 1, 1);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(31, 32));
        pushButton_2->setMaximumSize(QSize(31, 32));

        gridLayout->addWidget(pushButton_2, 1, 1, 1, 1);

        widget = new QWidget(groupBox);
        widget->setObjectName(QString::fromUtf8("widget"));

        gridLayout->addWidget(widget, 1, 2, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        horizontalLayout_2->addWidget(groupBox);


        retranslateUi(GridListWidget);

        QMetaObject::connectSlotsByName(GridListWidget);
    } // setupUi

    void retranslateUi(QWidget *GridListWidget)
    {
        GridListWidget->setWindowTitle(QApplication::translate("GridListWidget", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("GridListWidget", "Groups", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("GridListWidget", "+", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("GridListWidget", "-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GridListWidget: public Ui_GridListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDLISTWIDGET_H
