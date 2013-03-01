/********************************************************************************
** Form generated from reading UI file 'newanimation.ui'
**
** Created: Fri 1. Mar 14:52:58 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWANIMATION_H
#define UI_NEWANIMATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_NewAnimation
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_2;
    QLabel *iNumFramesLabel;
    QSpinBox *iNumFramesSpin;
    QLabel *iFrameFrequencyLabel;
    QSpinBox *iFrameFrequencySpin;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QSpinBox *iNumRowsSpin;
    QSpinBox *iNumColumnsSpin;
    QLabel *iNumColumnsLabel;
    QLabel *iNumRowsLabel;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NewAnimation)
    {
        if (NewAnimation->objectName().isEmpty())
            NewAnimation->setObjectName(QString::fromUtf8("NewAnimation"));
        NewAnimation->resize(280, 267);
        verticalLayout = new QVBoxLayout(NewAnimation);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(NewAnimation);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        iNumFramesLabel = new QLabel(groupBox);
        iNumFramesLabel->setObjectName(QString::fromUtf8("iNumFramesLabel"));
        iNumFramesLabel->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(iNumFramesLabel, 0, 0, 1, 1);

        iNumFramesSpin = new QSpinBox(groupBox);
        iNumFramesSpin->setObjectName(QString::fromUtf8("iNumFramesSpin"));
        iNumFramesSpin->setMinimumSize(QSize(71, 25));
        iNumFramesSpin->setMaximumSize(QSize(71, 25));
        iNumFramesSpin->setMinimum(1);
        iNumFramesSpin->setMaximum(999);
        iNumFramesSpin->setValue(100);

        gridLayout_2->addWidget(iNumFramesSpin, 0, 1, 1, 1);

        iFrameFrequencyLabel = new QLabel(groupBox);
        iFrameFrequencyLabel->setObjectName(QString::fromUtf8("iFrameFrequencyLabel"));
        iFrameFrequencyLabel->setMinimumSize(QSize(0, 25));

        gridLayout_2->addWidget(iFrameFrequencyLabel, 1, 0, 1, 1);

        iFrameFrequencySpin = new QSpinBox(groupBox);
        iFrameFrequencySpin->setObjectName(QString::fromUtf8("iFrameFrequencySpin"));
        iFrameFrequencySpin->setMinimumSize(QSize(71, 25));
        iFrameFrequencySpin->setMaximumSize(QSize(71, 25));
        iFrameFrequencySpin->setMinimum(1);
        iFrameFrequencySpin->setMaximum(9999);
        iFrameFrequencySpin->setSingleStep(1);
        iFrameFrequencySpin->setValue(1000);

        gridLayout_2->addWidget(iFrameFrequencySpin, 1, 1, 1, 1);


        horizontalLayout->addLayout(gridLayout_2);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(NewAnimation);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        iNumRowsSpin = new QSpinBox(groupBox_2);
        iNumRowsSpin->setObjectName(QString::fromUtf8("iNumRowsSpin"));
        iNumRowsSpin->setMinimumSize(QSize(57, 0));
        iNumRowsSpin->setMaximumSize(QSize(57, 16777215));
        iNumRowsSpin->setMinimum(1);
        iNumRowsSpin->setMaximum(100);
        iNumRowsSpin->setValue(8);

        gridLayout->addWidget(iNumRowsSpin, 0, 1, 1, 1);

        iNumColumnsSpin = new QSpinBox(groupBox_2);
        iNumColumnsSpin->setObjectName(QString::fromUtf8("iNumColumnsSpin"));
        iNumColumnsSpin->setMinimumSize(QSize(57, 0));
        iNumColumnsSpin->setMaximumSize(QSize(57, 16777215));
        iNumColumnsSpin->setMinimum(1);
        iNumColumnsSpin->setMaximum(100);
        iNumColumnsSpin->setValue(8);

        gridLayout->addWidget(iNumColumnsSpin, 1, 1, 1, 1);

        iNumColumnsLabel = new QLabel(groupBox_2);
        iNumColumnsLabel->setObjectName(QString::fromUtf8("iNumColumnsLabel"));
        iNumColumnsLabel->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(iNumColumnsLabel, 1, 0, 1, 1);

        iNumRowsLabel = new QLabel(groupBox_2);
        iNumRowsLabel->setObjectName(QString::fromUtf8("iNumRowsLabel"));
        iNumRowsLabel->setMinimumSize(QSize(0, 25));

        gridLayout->addWidget(iNumRowsLabel, 0, 0, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);


        verticalLayout->addWidget(groupBox_2);

        buttonBox = new QDialogButtonBox(NewAnimation);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        groupBox->raise();
        buttonBox->raise();
        groupBox_2->raise();

        retranslateUi(NewAnimation);
        QObject::connect(buttonBox, SIGNAL(accepted()), NewAnimation, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), NewAnimation, SLOT(reject()));

        QMetaObject::connectSlotsByName(NewAnimation);
    } // setupUi

    void retranslateUi(QDialog *NewAnimation)
    {
        NewAnimation->setWindowTitle(QApplication::translate("NewAnimation", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("NewAnimation", "Frame Information", 0, QApplication::UnicodeUTF8));
        iNumFramesLabel->setText(QApplication::translate("NewAnimation", "Number of frames :", 0, QApplication::UnicodeUTF8));
        iFrameFrequencyLabel->setText(QApplication::translate("NewAnimation", "Frame frequency (ms) :", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("NewAnimation", "Led Information", 0, QApplication::UnicodeUTF8));
        iNumColumnsLabel->setText(QApplication::translate("NewAnimation", "Number of columns :", 0, QApplication::UnicodeUTF8));
        iNumRowsLabel->setText(QApplication::translate("NewAnimation", "Number of rows :", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NewAnimation: public Ui_NewAnimation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWANIMATION_H
