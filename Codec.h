/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef LEDANIMCODEC_H
#define LEDANIMCODEC_H

#include <QObject>
#include <QList>
#include <QColor>
#include <QSize>

#include "Function.h"

namespace AnimatorModel {
class Animation;
class Led;
class Frame;
class Axis;
class FunctionValue;
class AxisData;
}

using namespace AnimatorModel;

namespace ImportExport {

class AnimChar {

public:
    AnimChar(const int value);
    AnimChar(const unsigned char value);
    AnimChar();

    int intValue() const;
    unsigned int unsignedIntValue() const;
    unsigned char unsignedCharValue() const;
    bool boolValue() const;

private:
    const char iValue;
};

class AnimStringChar : public AnimChar {

};

class AnimByteChar : public AnimChar {

};

class LedAnimCodec : public QObject {
    Q_OBJECT

public:
    explicit LedAnimCodec(Animation &animation);

    void writeAnimation(bool withPositions);

    void readHeader();
    void readAnimation();

protected:
    virtual const AnimChar readCharacter() const = 0;
    virtual void writeCharacter(AnimChar character) = 0;

    virtual void writeString(QString string) = 0;
    virtual bool isAnimatorFile() const = 0;

    virtual void writeControlCharacter(AnimChar character) = 0;
    virtual const AnimChar readControlCharacter() const = 0;

    virtual const QByteArray&  asByteArray() const = 0;
    virtual const QString     asString() const = 0;
    virtual const QString  headerString() const = 0;

    virtual void writePositionData() { }
    virtual QList<int> readPositionData(int* numRows, int* numColumns, int numLeds) { Q_UNUSED(numRows); Q_UNUSED(numColumns); Q_UNUSED(numLeds); QList<int> positions; return positions; }

    void writeHeader();

    void writeTimeAxisData();
    void readTimeAxisData();

    void writeValueAxisData(int iAxisNum);
    void readValueAxisData(int iAxisNum);

    void writeAxis(int iAxisNum);
    void readAxis(int iAxisNum);

    void writeRanges(AxisData& axisData);
    void readRanges(AxisData &axisData);

    virtual void writeColour(Frame& frame) = 0;
    virtual void writeColour(QColor colour) = 0;
    virtual const QColor readColour() const = 0;

    virtual void writeFunction(Frame& frame) = 0;
    virtual const FunctionValue& readFunction(Frame& frame) const = 0;

    virtual void writeFunctionData(Function function) = 0;
    virtual Function readFunctionData() const = 0;

    Animation& iAnimation;
    bool iVerbose;
};




}

#endif // LEDANIMCODEC_H
