#include "ByteArrayCodec.h"

#include "Led.h"
#include "Frame.h"
#include "Animation.h"
#include "ColourValue.h"
#include "FunctionValue.h"
#include "FunctionFrame.h"

#include "constants.h"
#include "exceptions.h"

using namespace ImportExport;
using namespace Exception;

LedAnimByteArrayCodec::LedAnimByteArrayCodec(Animation &animation) :
    LedAnimCodec(animation),
    iPosition(0) {}


LedAnimByteArrayCodec::LedAnimByteArrayCodec(Animation& animation, QByteArray bytes) :
    LedAnimCodec(animation),
    iByteArray(bytes),
    iPosition(0) {
}

void LedAnimByteArrayCodec::writeAnimation(bool withPositions) {
    LedAnimCodec::writeAnimation(withPositions);

    iAnimation.setSaved(true);
}

void LedAnimByteArrayCodec::readAnimation() {
    LedAnimCodec::readAnimation();

    iAnimation.setSaved(true);
}

const AnimChar LedAnimByteArrayCodec::readControlCharacter() const {
    char value = iByteArray.at(iPosition++);

    if(value != TERMINATING_BYTE &&
       value != HEADER_BYTE ) {
        throw IllegalArgumentException("LedAnimByteArrayCodec::readControlCharacter : not a control character");
    }

    return AnimChar(value);
}

const AnimChar LedAnimByteArrayCodec::readCharacter() const {
#ifdef CODEC_USE_ESCAPE_CHARS
    if(iByteArray.at(iPosition) == ESCAPE_BYTE) {
        AnimChar character(iByteArray.at(iPosition+1) ^ XOR_BYTE);
        iPosition = iPosition + 2;
        return character;
    } else {
        return AnimChar(iByteArray.at(iPosition++));
    }
#else
    return AnimChar(iByteArray.at(iPosition++));
#endif
}

void LedAnimByteArrayCodec::writeCharacter(AnimChar character) {
    char value = character.unsignedCharValue();

#ifdef CODEC_USE_ESCAPE_CHARS
    if(value == TERMINATING_BYTE ||
       value == ESCAPE_BYTE ||
       value == HEADER_BYTE ) {
        iByteArray.append(ESCAPE_BYTE);
        iByteArray.append(value ^ XOR_BYTE);
    } else {
        iByteArray.append(value);
    }
#else
    iByteArray.append(value);
#endif
}

bool LedAnimByteArrayCodec::isAnimatorFile() const {
    QString header;

    header.append(readCharacter().unsignedCharValue());
    header.append(readCharacter().unsignedCharValue());
    header.append(readCharacter().unsignedCharValue());
    header.append(readCharacter().unsignedCharValue());

    return header == headerString();
}

void LedAnimByteArrayCodec::writeString(QString string) {
    iByteArray.append(string);
}

void LedAnimByteArrayCodec::writeControlCharacter(AnimChar character) {
    char value = character.unsignedCharValue();
    if(value != TERMINATING_BYTE &&
       value != HEADER_BYTE ) {
        throw IllegalArgumentException("LedAnimByteArrayCodec::writeControlCharacter : not a control character");
    }

    iByteArray.append(value);
}

void LedAnimByteArrayCodec::writePositionData() {
    writeCharacter(iAnimation.numRows());
    writeCharacter(iAnimation.numColumns());

    int ledNum = INITIAL_LED;
    for(int i = 0; i < iAnimation.numLeds(); i++) {
        Position position;
        do {
            position = iAnimation.ledPosition(ledNum);
            if(!position.isValid()) {
                ledNum++;
            }
        } while(!position.isValid());

        writeInt(ledNum++);
        writeCharacter(position.row());
        writeCharacter(position.column());
    }
}

QList<int> LedAnimByteArrayCodec::readPositionData(int* numRows, int* numColumns, int numLeds) {
    QList<int> positions;

    unsigned int rows = readCharacter().unsignedIntValue();
    unsigned int columns = readCharacter().unsignedIntValue();

    for(int i = 0; i < rows * columns; i++) {
        positions.append(INVALID);
    }

    unsigned int ledNum;
    unsigned int row;
    unsigned int column;

    for(int i = 0; i < numLeds; i++) {

        unsigned char ledNumHigh = readCharacter().unsignedCharValue();
        unsigned char ledNumLow = readCharacter().unsignedCharValue();

        ledNum = ledNumHigh;
        ledNum |= ledNumLow << 8;
        row = readCharacter().unsignedCharValue();
        column = readCharacter().unsignedCharValue();

        if(row != INVALID && column != INVALID) {
            positions.replace((row*columns) + column, ledNum);
        }
    }

    *numRows = rows;
    *numColumns = columns;

    return positions;
}

void LedAnimByteArrayCodec::writeColour(Frame &frame) {
    const ColourValue& colourValue = static_cast<const ColourValue&>(frame.value());

    writeColour(colourValue.colour());
}

void LedAnimByteArrayCodec::writeColour(QColor colour) {
    int hue = colour.hue();
    char hueHigh = hue;
    char hueLow = hue >> 8;
    writeCharacter(hueHigh);
    writeCharacter(hueLow);
    writeCharacter(colour.saturation());
    writeCharacter(colour.value());
}

const QColor LedAnimByteArrayCodec::readColour() const {
    unsigned char hueHigh = readCharacter().unsignedCharValue();
    char hueLow = readCharacter().unsignedCharValue();

    int hue = hueHigh;
    hue |= hueLow << 8;

    unsigned char saturation = readCharacter().unsignedCharValue();
    unsigned char value = readCharacter().unsignedCharValue();

    QColor colour = QColor::fromHsv(hue, saturation, value);

    if(!colour.isValid()) {
        qDebug("invalid colour");
    }

    return colour;
}

void LedAnimByteArrayCodec::writeFunction(Frame& frame) {
    const FunctionValue& value = static_cast<const FunctionValue&>(frame.value());

    writeFunctionData(value.function());
    writeCharacter(value.number());
}

const FunctionValue& LedAnimByteArrayCodec::readFunction(Frame& frame) const {
    Function function = readFunctionData();

    readCharacter().intValue();
    return *(new FunctionValue(&frame,
                            function ,iAnimation.addFunction(function)));
                    //));
}

void LedAnimByteArrayCodec::writeFunctionData(Function function) {
    float redIncrement = (float)function.redIncrement();
    unsigned char *p = reinterpret_cast<unsigned char *>(&redIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        writeCharacter(p[i]);
    }

    float greenIncrement = (float)function.greenIncrement();
    p = reinterpret_cast<unsigned char *>(&greenIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        writeCharacter(p[i]);
    }

    float blueIncrement = (float)function.blueIncrement();
    p = reinterpret_cast<unsigned char *>(&blueIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        writeCharacter(p[i]);
    }
}

Function LedAnimByteArrayCodec::readFunctionData() const{
    float redIncrement = 0;
    unsigned char *p = reinterpret_cast<unsigned char *>(&redIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        *(p+i) = readCharacter().unsignedCharValue();
    }

    float greenIncrement = 0;
    p = reinterpret_cast<unsigned char *>(&greenIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        *(p+i) = readCharacter().unsignedCharValue();
    }

    float blueIncrement = 0;
    p = reinterpret_cast<unsigned char *>(&blueIncrement);
    for (std::size_t i = 0; i != sizeof(float); ++i) {
        *(p+i) = readCharacter().unsignedCharValue();
    }

    Function function = Function(redIncrement,
                                 greenIncrement,
                                blueIncrement);

    return function;
}

const QString LedAnimByteArrayCodec::asString() const {
    return "";
}

const QByteArray &LedAnimByteArrayCodec::asByteArray() const {
    return iByteArray;
}

LedAnimSimpleByteArrayCodec::LedAnimSimpleByteArrayCodec(Animation &animation) :
    LedAnimByteArrayCodec(animation) {}


LedAnimSimpleByteArrayCodec::LedAnimSimpleByteArrayCodec(Animation& animation, QByteArray bytes) :
    LedAnimByteArrayCodec(animation, bytes) {
}


void LedAnimSimpleByteArrayCodec::writeColour(Frame &frame) {
    const ColourValue& colourValue = static_cast<const ColourValue&>(frame.value());

    writeColour(colourValue.colour());
}

void LedAnimSimpleByteArrayCodec::writeColour(QColor colour) {
    writeCharacter(colour.red());
    writeCharacter(colour.green());
    writeCharacter(colour.blue());
}

const QColor LedAnimSimpleByteArrayCodec::readColour() const {
    return QColor();
}
void LedAnimSimpleByteArrayCodec::writeFunction(Frame& frame) {
    const FunctionValue& value = static_cast<const FunctionValue&>(frame.value());

    writeCharacter(value.number());
}

const FunctionValue& LedAnimSimpleByteArrayCodec::readFunction(Frame& frame) const {
    Q_UNUSED(frame);
}

void LedAnimSimpleByteArrayCodec::writeFunctionData(Function function) {
    float multiplier = 256 * 256;

    float redIncrementF = (float)function.redIncrement();
    double redIncrementD = (double)redIncrementF;
    redIncrementD *= multiplier;
    redIncrementD = round(redIncrementD);

    if(redIncrementD > 0) {
        redIncrementD--;
    } else if(redIncrementD < 0) {
        redIncrementD++;
    }

    int redIncrement = (int)redIncrementD;

    unsigned char redIncrement1 = redIncrement;
    unsigned char redIncrement2 = redIncrement >> 8;
    unsigned char redIncrement3 = redIncrement >> 16;
    unsigned char redIncrement4 = redIncrement >> 24;
    writeCharacter(redIncrement1);
    writeCharacter(redIncrement2);
    writeCharacter(redIncrement3);
    writeCharacter(redIncrement4);

    float greenIncrementF = (float)function.greenIncrement();
    double greenIncrementD = (double)greenIncrementF;
    greenIncrementD *= multiplier;
    greenIncrementD = round(greenIncrementD);

    if(greenIncrementD > 0) {
        greenIncrementD--;
    } else if(greenIncrementD < 0) {
        greenIncrementD++;
    }

    int greenIncrement = (int)greenIncrementD;

    unsigned char greenIncrement1 = greenIncrement;
    unsigned char greenIncrement2 = greenIncrement >> 8;
    unsigned char greenIncrement3 = greenIncrement >> 16;
    unsigned char greenIncrement4 = greenIncrement >> 24;
    writeCharacter(greenIncrement1);
    writeCharacter(greenIncrement2);
    writeCharacter(greenIncrement3);
    writeCharacter(greenIncrement4);

    float blueIncrementF = (float)function.blueIncrement();
    double blueIncrementD = (double)blueIncrementF;
    blueIncrementD *= multiplier;
    blueIncrementD = round(blueIncrementD);

    if(blueIncrementD > 0) {
        blueIncrementD--;
    } else if(blueIncrementD < 0) {
        blueIncrementD++;
    }

    int blueIncrement = (int)blueIncrementD;

    unsigned char blueIncrement1 = blueIncrement;
    unsigned char blueIncrement2 = blueIncrement >> 8;
    unsigned char blueIncrement3 = blueIncrement >> 16;
    unsigned char blueIncrement4 = blueIncrement >> 24;
    writeCharacter(blueIncrement1);
    writeCharacter(blueIncrement2);
    writeCharacter(blueIncrement3);
    writeCharacter(blueIncrement4);
}

Function LedAnimSimpleByteArrayCodec::readFunctionData() const {
    return Function();
}
