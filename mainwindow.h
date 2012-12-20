#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSlider>
#include <QSplitter>

class Led;
class LedWidget;
class LedGridWidget;
class PlayInfoWidget;
class Engine;
class AnimationDetailsWidget;
class Animation;

class MainWindow : public QMainWindow
{
    Q_OBJECT    
public:
    explicit MainWindow(Engine& engine);

    void readSettings();
    void writeSettings();

    Animation& animation();

public slots:

protected:
    void closeEvent(QCloseEvent *event);
    
private:
    Engine&         iEngine;

   // QWidget*        iKittenWidget;

    LedGridWidget*          iLedGridWidget;
    PlayInfoWidget*         iPlayInfoWidget;
    AnimationDetailsWidget*     iFrameDetailsWidget;
};

#endif // MAINWINDOW_H
