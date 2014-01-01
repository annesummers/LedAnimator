#ifndef FRAMEDETAILS_H
#define FRAMEDETAILS_H

#include <QWidget>

namespace Ui {
class FrameDetailsWidget;
}

namespace AnimatorUi {

class FrameDetailsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit FrameDetailsWidget(QWidget *parent = 0);
    ~FrameDetailsWidget();

public slots:
    void highValueChanged(int highValue);

private slots:
    void currentFrameChanged(int currentFrame);
 //   void framesInserted(int numFrames, int framesAdded);
    
private:
    Ui::FrameDetailsWidget *ui;
};

}
#endif // FRAMEDETAILS_H
