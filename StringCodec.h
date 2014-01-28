#ifndef STRINGCODEC_H
#define STRINGCODEC_H

#include "Codec.h"

using namespace AnimatorModel;

namespace ImportExport {

class LedAnimStringCodec: public LedAnimCodec {
    Q_OBJECT
public:
    explicit LedAnimStringCodec(Animation &animation);

    const QString asString() const;

protected:
    const AnimChar readCharacter() const;

    void writeCharacter(AnimChar character);
    virtual void writeString(QString string);

    inline const QString headerString() const { return "LAEX"; }

    bool isAnimatorFile() const;

    void writeControlCharacter(AnimChar character);
    const AnimChar readControlCharacter() const { return AnimChar(); }

    const QByteArray &asByteArray() const;

    virtual void writeColour(Frame& frame);
    virtual void writeColour(QColor colour);
    virtual const QColor readColour() const;

    void writeFunction(Frame& frame);
    const FunctionValue& readFunction(Frame& frame) const;

    virtual void writeFunctionData(Function function);
    virtual Function readFunctionData() const;

private:
    QString iString;
    int iLastReturn;
};

}


#endif // STRINGCODEC_H
