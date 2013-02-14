/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef PLAYINFOWIDGET_H
#define PLAYINFOWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QLabel>

class Animation;

namespace AnimatorUi {

class PlayInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayInfoWidget(QWidget* parent, Animation &animation);

private slots:
    void playClicked();
    void firstClicked();
    void previousClicked();
    void nextClicked();
    void lastClicked();

    void currentFrameChanged(int currentFrame);

private:
    Animation&      iAnimation;

    QPushButton*    iFirstButton;
    QPushButton*    iPreviousButton;
    QPushButton*    iPlayButton;
    QPushButton*    iNextButton;
    QPushButton*    iLastButton;
    QLabel*         iFrameNumberLabel;
};
}

#endif // PLAYINFOWIDGET_H
