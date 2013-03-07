/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef PLAYINFOWIDGET_H
#define PLAYINFOWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QToolButton>
#include <QLabel>

namespace AnimatorModel {
class Animation;
}

using namespace AnimatorModel;

namespace AnimatorUi {

class PlayInfoWidget : public QWidget {
    Q_OBJECT

public:
    explicit PlayInfoWidget(QWidget* parent, Animation &animation);

private slots:
    void playClicked();
    void firstClicked();
    void previousClicked();
    void nextClicked();
    void lastClicked();
    void repeatClicked();

    void stopped();

    void currentFrameChanged(int currentFrame);

private:
    Animation&      iAnimation;

    QToolButton*    iFirstButton;
    QToolButton*    iPreviousButton;
    QToolButton*    iPlayButton;
    QToolButton*    iNextButton;
    QToolButton*    iLastButton;
    QToolButton*    iRepeatButton;
    //QLabel*         iFrameNumberLabel;
};
}

#endif // PLAYINFOWIDGET_H
