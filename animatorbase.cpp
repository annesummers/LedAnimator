/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#include "animatorbase.h"

#include "animation.h"

AnimatorBase::AnimatorBase(QObject *parent, Animation &animation) :
    QObject(parent),
    iAnimation(animation)
{
}
