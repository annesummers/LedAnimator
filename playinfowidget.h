#ifndef PLAYINFOWIDGET_H
#define PLAYINFOWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QLabel>

class MainWindow;

class PlayInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayInfoWidget(QWidget* parent, MainWindow& mainWindow);
    
signals:
    
public slots:
    void playClicked();
    void firstClicked();

    void currentFrameChanged(int currentFrame);

private:
    MainWindow&     iMainWindow;

    QPushButton*    iFirstButton;
    QPushButton*    iPlayButton;
    QLabel*         iFrameNumberLabel;
};

#endif // PLAYINFOWIDGET_H
