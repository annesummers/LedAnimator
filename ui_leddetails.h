/********************************************************************************
** Form generated from reading UI file 'leddetails.ui'
**
** Created: Thu 7. Mar 12:42:43 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEDDETAILS_H
#define UI_LEDDETAILS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LedDetailsWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QLabel *ledNumber;
    QGroupBox *positionGroupBox;
    QFormLayout *formLayout_2;
    QLabel *label_7;
    QLabel *ledRow;
    QLabel *ledColumn;
    QLabel *label_8;
    QGroupBox *colourGroupBox;
    QFormLayout *formLayout;
    QLabel *label_4;
    QLabel *ledHue;
    QLabel *ledSaturation;
    QLabel *label_6;
    QLabel *ledValue;
    QLabel *label_5;

    void setupUi(QWidget *LedDetailsWidget)
    {
        if (LedDetailsWidget->objectName().isEmpty())
            LedDetailsWidget->setObjectName(QString::fromUtf8("LedDetailsWidget"));
        LedDetailsWidget->resize(185, 266);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LedDetailsWidget->sizePolicy().hasHeightForWidth());
        LedDetailsWidget->setSizePolicy(sizePolicy);
        LedDetailsWidget->setMinimumSize(QSize(185, 266));
        LedDetailsWidget->setMaximumSize(QSize(185, 266));
        horizontalLayout = new QHBoxLayout(LedDetailsWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(LedDetailsWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ledNumber = new QLabel(groupBox);
        ledNumber->setObjectName(QString::fromUtf8("ledNumber"));

        verticalLayout->addWidget(ledNumber);

        positionGroupBox = new QGroupBox(groupBox);
        positionGroupBox->setObjectName(QString::fromUtf8("positionGroupBox"));
        positionGroupBox->setMinimumSize(QSize(0, 0));
        positionGroupBox->setMaximumSize(QSize(16777215, 16777215));
        formLayout_2 = new QFormLayout(positionGroupBox);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        label_7 = new QLabel(positionGroupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_7);

        ledRow = new QLabel(positionGroupBox);
        ledRow->setObjectName(QString::fromUtf8("ledRow"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, ledRow);

        ledColumn = new QLabel(positionGroupBox);
        ledColumn->setObjectName(QString::fromUtf8("ledColumn"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, ledColumn);

        label_8 = new QLabel(positionGroupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_8);


        verticalLayout->addWidget(positionGroupBox);

        colourGroupBox = new QGroupBox(groupBox);
        colourGroupBox->setObjectName(QString::fromUtf8("colourGroupBox"));
        colourGroupBox->setMinimumSize(QSize(113, 101));
        colourGroupBox->setMaximumSize(QSize(16777215, 101));
        formLayout = new QFormLayout(colourGroupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label_4 = new QLabel(colourGroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_4);

        ledHue = new QLabel(colourGroupBox);
        ledHue->setObjectName(QString::fromUtf8("ledHue"));

        formLayout->setWidget(0, QFormLayout::FieldRole, ledHue);

        ledSaturation = new QLabel(colourGroupBox);
        ledSaturation->setObjectName(QString::fromUtf8("ledSaturation"));

        formLayout->setWidget(1, QFormLayout::FieldRole, ledSaturation);

        label_6 = new QLabel(colourGroupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_6);

        ledValue = new QLabel(colourGroupBox);
        ledValue->setObjectName(QString::fromUtf8("ledValue"));

        formLayout->setWidget(2, QFormLayout::FieldRole, ledValue);

        label_5 = new QLabel(colourGroupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);


        verticalLayout->addWidget(colourGroupBox);

        ledNumber->raise();
        colourGroupBox->raise();
        positionGroupBox->raise();
        label_5->raise();
        label_5->raise();

        horizontalLayout->addWidget(groupBox);


        retranslateUi(LedDetailsWidget);

        QMetaObject::connectSlotsByName(LedDetailsWidget);
    } // setupUi

    void retranslateUi(QWidget *LedDetailsWidget)
    {
        LedDetailsWidget->setWindowTitle(QApplication::translate("LedDetailsWidget", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("LedDetailsWidget", "Led details", 0, QApplication::UnicodeUTF8));
        ledNumber->setText(QString());
        positionGroupBox->setTitle(QApplication::translate("LedDetailsWidget", "Position", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("LedDetailsWidget", "Row:", 0, QApplication::UnicodeUTF8));
        ledRow->setText(QString());
        ledColumn->setText(QString());
        label_8->setText(QApplication::translate("LedDetailsWidget", "Column:", 0, QApplication::UnicodeUTF8));
        colourGroupBox->setTitle(QApplication::translate("LedDetailsWidget", "Colour", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("LedDetailsWidget", "Hue:", 0, QApplication::UnicodeUTF8));
        ledHue->setText(QString());
        ledSaturation->setText(QString());
        label_6->setText(QApplication::translate("LedDetailsWidget", "Value:", 0, QApplication::UnicodeUTF8));
        ledValue->setText(QString());
        label_5->setText(QApplication::translate("LedDetailsWidget", "Saturation:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LedDetailsWidget: public Ui_LedDetailsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEDDETAILS_H
