#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSlider>
#include <QSplitter>

class Led;
class LedWidget;
class LedGridContainerWidget;
class PlayInfoWidget;
class Engine;
class FrameDetailsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT    
public:
    explicit MainWindow(Engine& engine);
    ~MainWindow();

    void readSettings();
    void writeSettings();

public slots:
    void setCurrentFrame(int frame);

    void playClicked();

protected:
    void closeEvent(QCloseEvent *event);
    
private:
    Engine&         iEngine;

   // QWidget*        iKittenWidget;

    LedGridContainerWidget* iLedGridWidget;
    PlayInfoWidget*         iPlayInfoWidget;
    FrameDetailsWidget*     iFrameDetailsWidget;
};

#endif // MAINWINDOW_H
