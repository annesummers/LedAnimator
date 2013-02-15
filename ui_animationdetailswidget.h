/********************************************************************************
** Form generated from reading UI file 'animationdetailswidget.ui'
**
** Created: Fri 15. Feb 10:53:32 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANIMATIONDETAILSWIDGET_H
#define UI_ANIMATIONDETAILSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AnimationDetailsWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *iScrollArea;
    QWidget *iScrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QWidget *widget;
    QSlider *horizontalSlider;
    QWidget *widget_2;
    QWidget *widget_3;
    QWidget *widget_4;
    QWidget *widget_5;
    QWidget *widget_6;

    void setupUi(QWidget *AnimationDetailsWidget)
    {
        if (AnimationDetailsWidget->objectName().isEmpty())
            AnimationDetailsWidget->setObjectName(QString::fromUtf8("AnimationDetailsWidget"));
        AnimationDetailsWidget->resize(557, 331);
        AnimationDetailsWidget->setAcceptDrops(true);
        horizontalLayout = new QHBoxLayout(AnimationDetailsWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        iScrollArea = new QScrollArea(AnimationDetailsWidget);
        iScrollArea->setObjectName(QString::fromUtf8("iScrollArea"));
        iScrollArea->setWidgetResizable(true);
        iScrollAreaWidgetContents = new QWidget();
        iScrollAreaWidgetContents->setObjectName(QString::fromUtf8("iScrollAreaWidgetContents"));
        iScrollAreaWidgetContents->setGeometry(QRect(0, 0, 531, 305));
        horizontalLayout_2 = new QHBoxLayout(iScrollAreaWidgetContents);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget = new QWidget(iScrollAreaWidgetContents);
        widget->setObjectName(QString::fromUtf8("widget"));

        gridLayout->addWidget(widget, 0, 0, 1, 1);

        horizontalSlider = new QSlider(iScrollAreaWidgetContents);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 0, 1, 1, 1);

        widget_2 = new QWidget(iScrollAreaWidgetContents);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));

        gridLayout->addWidget(widget_2, 0, 2, 1, 1);

        widget_3 = new QWidget(iScrollAreaWidgetContents);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));

        gridLayout->addWidget(widget_3, 1, 0, 1, 1);

        widget_4 = new QWidget(iScrollAreaWidgetContents);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_4->sizePolicy().hasHeightForWidth());
        widget_4->setSizePolicy(sizePolicy);

        gridLayout->addWidget(widget_4, 1, 1, 1, 1);

        widget_5 = new QWidget(iScrollAreaWidgetContents);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));

        gridLayout->addWidget(widget_5, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        widget_6 = new QWidget(iScrollAreaWidgetContents);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_6->sizePolicy().hasHeightForWidth());
        widget_6->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(widget_6);


        horizontalLayout_2->addLayout(verticalLayout);

        iScrollArea->setWidget(iScrollAreaWidgetContents);

        horizontalLayout->addWidget(iScrollArea);


        retranslateUi(AnimationDetailsWidget);

        QMetaObject::connectSlotsByName(AnimationDetailsWidget);
    } // setupUi

    void retranslateUi(QWidget *AnimationDetailsWidget)
    {
        AnimationDetailsWidget->setWindowTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class AnimationDetailsWidget: public Ui_AnimationDetailsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANIMATIONDETAILSWIDGET_H
