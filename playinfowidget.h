#ifndef PLAYINFOWIDGET_H
#define PLAYINFOWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QLabel>

class Animation;

class PlayInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayInfoWidget(QWidget* parent, Animation &animation);
    
signals:
    
public slots:
    void playClicked();
    void firstClicked();

    void currentFrameChanged(int currentFrame);

private:
    Animation&      iAnimation;

    QPushButton*    iFirstButton;
    QPushButton*    iPlayButton;
    QLabel*         iFrameNumberLabel;
};

#endif // PLAYINFOWIDGET_H
