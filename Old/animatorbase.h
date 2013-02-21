/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef ANIMATORBASE_H
#define ANIMATORBASE_H

#include <QObject>

class Animation;

class AnimatorBase : public QObject
{
    Q_OBJECT
public:
    explicit AnimatorBase(QObject *parent, Animation& animation);
    
signals:
    
public slots:

protected:
    Animation& animation() { return iAnimation; }

private:
    Animation& iAnimation;
};

#endif // ANIMATORBASE_H
