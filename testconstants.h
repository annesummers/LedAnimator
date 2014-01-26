/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef TESTCONSTANTS_H
#define TESTCONSTANTS_H

#include <QMetaType>

#include "Position.h"

typedef QList<int> IntList;
Q_DECLARE_METATYPE ( IntList )

typedef QList<Position> PositionList;

Q_DECLARE_METATYPE ( PositionList )

typedef QList<Function> FunctionList;

Q_DECLARE_METATYPE ( FunctionList )

const QString TEST_MIME_TYPE = "ledanimatortests/x-testitemdata";

#endif // TESTCONSTANTS_H
