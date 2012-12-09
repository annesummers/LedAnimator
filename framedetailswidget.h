#ifndef FRAMEDETAILSWIDGET_H
#define FRAMEDETAILSWIDGET_H

#include <QWidget>
#include <QSlider>

class MainWindow;
class Led;

class FrameDetailsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FrameDetailsWidget(QWidget* parent, MainWindow& mainWindow);
    
signals:
    
public slots:
    void newLed(Led& led);
    void currentFrameChanged(int currentFrame);
    void numFramesChanged(int numFrames);
    void ledColourChanged(int row, int column, int frame);

protected:
    void resizeEvent(QResizeEvent*);

private:
    MainWindow&     iMainWindow;

    QSlider*        iFrameSlider;
};

#endif // FRAMEDETAILSWIDGET_H
