/********************************************************************************
** Form generated from reading UI file 'playinfowidget.ui'
**
** Created: Fri 15. Feb 10:53:32 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLAYINFOWIDGET_H
#define UI_PLAYINFOWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlayInfoWidget
{
public:
    QGridLayout *gridLayout;
    QPushButton *iFirstButton;
    QLabel *iFrameNumberLabel;
    QPushButton *iLastButton;
    QPushButton *iPreviousButton;
    QPushButton *iPlayButton;
    QPushButton *iNextButton;

    void setupUi(QWidget *PlayInfoWidget)
    {
        if (PlayInfoWidget->objectName().isEmpty())
            PlayInfoWidget->setObjectName(QString::fromUtf8("PlayInfoWidget"));
        PlayInfoWidget->resize(254, 78);
        PlayInfoWidget->setMinimumSize(QSize(254, 78));
        PlayInfoWidget->setMaximumSize(QSize(254, 78));
        gridLayout = new QGridLayout(PlayInfoWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        iFirstButton = new QPushButton(PlayInfoWidget);
        iFirstButton->setObjectName(QString::fromUtf8("iFirstButton"));
        iFirstButton->setMinimumSize(QSize(95, 32));
        iFirstButton->setMaximumSize(QSize(95, 32));

        gridLayout->addWidget(iFirstButton, 0, 0, 1, 1);

        iFrameNumberLabel = new QLabel(PlayInfoWidget);
        iFrameNumberLabel->setObjectName(QString::fromUtf8("iFrameNumberLabel"));
        iFrameNumberLabel->setMinimumSize(QSize(55, 20));
        iFrameNumberLabel->setMaximumSize(QSize(55, 20));

        gridLayout->addWidget(iFrameNumberLabel, 0, 1, 1, 1);

        iLastButton = new QPushButton(PlayInfoWidget);
        iLastButton->setObjectName(QString::fromUtf8("iLastButton"));
        iLastButton->setMinimumSize(QSize(72, 32));
        iLastButton->setMaximumSize(QSize(72, 32));

        gridLayout->addWidget(iLastButton, 0, 2, 1, 1);

        iPreviousButton = new QPushButton(PlayInfoWidget);
        iPreviousButton->setObjectName(QString::fromUtf8("iPreviousButton"));
        iPreviousButton->setMinimumSize(QSize(95, 32));
        iPreviousButton->setMaximumSize(QSize(95, 32));

        gridLayout->addWidget(iPreviousButton, 1, 0, 1, 1);

        iPlayButton = new QPushButton(PlayInfoWidget);
        iPlayButton->setObjectName(QString::fromUtf8("iPlayButton"));
        iPlayButton->setMinimumSize(QSize(67, 32));
        iPlayButton->setMaximumSize(QSize(67, 32));

        gridLayout->addWidget(iPlayButton, 1, 1, 1, 1);

        iNextButton = new QPushButton(PlayInfoWidget);
        iNextButton->setObjectName(QString::fromUtf8("iNextButton"));
        iNextButton->setMinimumSize(QSize(72, 32));
        iNextButton->setMaximumSize(QSize(72, 32));

        gridLayout->addWidget(iNextButton, 1, 2, 1, 1);


        retranslateUi(PlayInfoWidget);

        QMetaObject::connectSlotsByName(PlayInfoWidget);
    } // setupUi

    void retranslateUi(QWidget *PlayInfoWidget)
    {
        PlayInfoWidget->setWindowTitle(QString());
        iFirstButton->setText(QApplication::translate("PlayInfoWidget", "First", 0, QApplication::UnicodeUTF8));
        iFrameNumberLabel->setText(QString());
        iLastButton->setText(QApplication::translate("PlayInfoWidget", "Last", 0, QApplication::UnicodeUTF8));
        iPreviousButton->setText(QApplication::translate("PlayInfoWidget", "Previous", 0, QApplication::UnicodeUTF8));
        iPlayButton->setText(QApplication::translate("PlayInfoWidget", "Play", 0, QApplication::UnicodeUTF8));
        iNextButton->setText(QApplication::translate("PlayInfoWidget", "Next", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PlayInfoWidget: public Ui_PlayInfoWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLAYINFOWIDGET_H
