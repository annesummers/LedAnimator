/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animatorwidgetbase.h"

using namespace Ui;

AnimatorWidgetBase::AnimatorWidgetBase(QWidget *parent, Animation& animation) :
    QWidget(parent),
    iAnimation(animation)
{
}
