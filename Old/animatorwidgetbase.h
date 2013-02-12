/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef ANIMATORWIDGETBASE_H
#define ANIMATORWIDGETBASE_H

#include <QWidget>

class Animation;

namespace Ui {

class AnimatorWidgetBase : public QWidget
{
    Q_OBJECT
public:
    explicit AnimatorWidgetBase(QWidget *parent, Animation &animation);
    
signals:
    
public slots:

protected:
    Animation& animation() { return iAnimation; }

private:
    Animation& iAnimation;
};
}

#endif // ANIMATORWIDGETBASE_H
