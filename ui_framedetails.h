/********************************************************************************
** Form generated from reading UI file 'framedetails.ui'
**
** Created: Thu 7. Mar 12:42:43 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAMEDETAILS_H
#define UI_FRAMEDETAILS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FrameDetailsWidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *currentFrame;
    QLabel *slash;
    QLabel *numFrames;

    void setupUi(QWidget *FrameDetailsWidget)
    {
        if (FrameDetailsWidget->objectName().isEmpty())
            FrameDetailsWidget->setObjectName(QString::fromUtf8("FrameDetailsWidget"));
        FrameDetailsWidget->resize(167, 72);
        horizontalLayout_2 = new QHBoxLayout(FrameDetailsWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(FrameDetailsWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(100, 61));
        groupBox->setMaximumSize(QSize(140, 61));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        currentFrame = new QLabel(groupBox);
        currentFrame->setObjectName(QString::fromUtf8("currentFrame"));

        horizontalLayout->addWidget(currentFrame);

        slash = new QLabel(groupBox);
        slash->setObjectName(QString::fromUtf8("slash"));

        horizontalLayout->addWidget(slash);

        numFrames = new QLabel(groupBox);
        numFrames->setObjectName(QString::fromUtf8("numFrames"));

        horizontalLayout->addWidget(numFrames);


        horizontalLayout_2->addWidget(groupBox);


        retranslateUi(FrameDetailsWidget);

        QMetaObject::connectSlotsByName(FrameDetailsWidget);
    } // setupUi

    void retranslateUi(QWidget *FrameDetailsWidget)
    {
        FrameDetailsWidget->setWindowTitle(QApplication::translate("FrameDetailsWidget", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("FrameDetailsWidget", "FrameDetails", 0, QApplication::UnicodeUTF8));
        currentFrame->setText(QString());
        slash->setText(QApplication::translate("FrameDetailsWidget", "/", 0, QApplication::UnicodeUTF8));
        numFrames->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FrameDetailsWidget: public Ui_FrameDetailsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAMEDETAILS_H
