#ifndef BYTEARRAYCODEC_H
#define BYTEARRAYCODEC_H

#include "Codec.h"

const QString LED_ANIMATOR_HEADER_STRING = "LAAN";
const QString LED_CODEC_HEADER_STRING = "LAEX";

using namespace AnimatorModel;

namespace ImportExport {
class LedAnimByteArrayCodec: public LedAnimCodec {
    Q_OBJECT
public:
    explicit LedAnimByteArrayCodec(Animation& animation);
    LedAnimByteArrayCodec(Animation& animation, QByteArray bytes);

    void writeAnimation(bool withPositions);
    void readAnimation();

    const QString asString() const;
    const QByteArray& asByteArray() const;

protected:
    const AnimChar readCharacter() const;
    void writeCharacter(AnimChar character);

    void writeString(QString string);

    void writeControlCharacter(AnimChar character);
    const AnimChar readControlCharacter() const;

    inline const QString headerString() const { return LED_ANIMATOR_HEADER_STRING; }

    bool isAnimatorFile() const;

    void writePositionData();
    QList<int> readPositionData(int *numRows, int *numColumns, int numLeds);

    virtual void writeFunction(Frame& frame);
    virtual const FunctionValue& readFunction(Frame& frame) const;

    virtual void writeFunctionData(Function function);
    virtual Function readFunctionData() const;

    virtual void writeColour(Frame& frame);
    virtual void writeColour(QColor colour);
    virtual const QColor readColour() const;

private:
    QByteArray  iByteArray;
    mutable int iPosition;
};

class LedAnimSimpleByteArrayCodec: public LedAnimByteArrayCodec {
    Q_OBJECT
public:
    explicit LedAnimSimpleByteArrayCodec(Animation& animation);
    LedAnimSimpleByteArrayCodec(Animation& animation, QByteArray bytes);

    inline const QString headerString() const { return LED_CODEC_HEADER_STRING; }

    virtual void writeFunction(Frame& frame);
    virtual const FunctionValue& readFunction(Frame& frame) const;

    virtual void writeFunctionData(Function function);
    virtual Function readFunctionData() const;

    virtual void writeColour(Frame& frame);
    virtual void writeColour(QColor colour);
    virtual const QColor readColour() const;
};

}

#endif // BYTEARRAYCODEC_H
