/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

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
    
    const QColor colour() const;
    void setColour(QColor colour);

signals:
    void colourChanged();
    
public slots:

private:
    QColor iColour;
};

#endif // FRAME_H
