#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QColor>

class Frame : public QObject
{
    Q_OBJECT
public:
    explicit Frame(QObject *parent);
    Frame(QObject* parent, QColor colour);
    
    QColor colour();
    void setColour(QColor colour);

signals:
    void colourChanged();
    
public slots:

private:
    QColor iColour;

    
};

#endif // FRAME_H
