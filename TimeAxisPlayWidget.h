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
class TimeAxis;
}

using namespace AnimatorModel;

namespace AnimatorUi {

class TimeAxisPlayWidget : public QWidget {
    Q_OBJECT

public:
    explicit TimeAxisPlayWidget(QWidget* parent, Animation &animation, AnimatorModel::TimeAxis &timeAxis);

private slots:
    void play();
    void first();
    void previous();
    void next();
    void last();
    void repeat();

    void stop();

    void handleCurrentFrameChanged(int currentFrame);

private:
    Animation&      iAnimation;
    TimeAxis&       iTimeAxis;

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
