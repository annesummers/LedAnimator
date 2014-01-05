/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#include "Codec.h"

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

AnimChar::AnimChar(const int value) :
    iValue(value) { }

AnimChar::AnimChar(const unsigned char value) :
    iValue(value) { }

AnimChar::AnimChar() :
    iValue(INVALID){

}

const int AnimChar::intValue() const {
    return static_cast<int>(iValue);
}

const unsigned int AnimChar::unsignedIntValue() const {
    return static_cast<unsigned int>(iValue);
}

const bool AnimChar::boolValue() const {
    return static_cast<bool>(iValue);
}

const unsigned char AnimChar::unsignedCharValue() const {
    return static_cast<unsigned char>(iValue);
}

LedAnimCodec::LedAnimCodec(Animation& animation) :
    QObject(&animation),
    iAnimation(animation){
}

void LedAnimCodec::writeAnimation(bool withPositions) {
    if(withPositions) {
        iWriteLedNumber = true;
    }

    writeControlCharacter(HEADER_BYTE);

    int numLeds = iAnimation.numLeds();
    char numLedsHigh = numLeds;
    char numLedsLow = numLeds >> 8;
    writeCharacter(numLedsHigh);
    writeCharacter(numLedsLow);

    if(withPositions) {
        writePositionData();
    }

    writeCharacter(iAnimation.numFunctions());
    for(int i = 0; i < iAnimation.numFunctions(); i++) {
        writeFunctionData(iAnimation.functionAt(i));
    }

    writeCharacter(iAnimation.numValueAxes());

    writeAxis(kTimeAxisNum);

    for(int i = 0; i < iAnimation.numValueAxes(); i++) {
        writeAxis(i);
    }


    writeAxisData(kTimeAxisNum);

    writeControlCharacter(TERMINATING_BYTE);
}

void LedAnimCodec::readAnimation() {
    if (readCharacter().unsignedCharValue() != HEADER_BYTE) {
        throw new InvalidAnimationException("No header byte");
    }

    char numLedsHigh = readCharacter().unsignedCharValue();
    char numLedsLow = readCharacter().unsignedCharValue();

    int numLeds =  numLedsHigh |= numLedsLow << 8;

    int numRows;
    int numColumns;
    QList<int> positions = readPositionData(&numRows, &numColumns, numLeds);

    iAnimation.newAnimation(numRows,
                        numColumns ,
                        numLeds,
                        positions);

    int numFunctions = readCharacter().intValue();
    for(int i = 0; i < numFunctions; i++) {
        readFunctionData();
    }

    int numAxes = readCharacter().intValue();

    readAxis(kTimeAxisNum);

    for(int i = 0; i < numAxes; i++) {
        readAxis(i);
    }


    readAxisData(kTimeAxisNum);

    if (readCharacter().unsignedCharValue() != TERMINATING_BYTE) {
        throw new InvalidAnimationException("No terminating byte");
    }
}


void LedAnimCodec::writeAxis(int axisNum) {
    Axis* axis = NULL;

    if(axisNum == kTimeAxisNum) {
        axis = iAnimation.timeAxis();

        writeCharacter(kTimeAxis);
        writeCharacter(axis->priority());
        writeCharacter(axis->isOpaque());

        writeCharacter(axis->lowValue());
        writeCharacter(axis->highValue());
        writeCharacter(iAnimation.timeAxis()->speed());
    } else {
        axis = &iAnimation.axisAt(axisNum);

        writeCharacter(kValueAxis);
        writeCharacter(axis->priority());
        writeCharacter(axis->isOpaque());

        writeCharacter(axis->lowValue());
        writeCharacter(axis->highValue());
        writeCharacter(iAnimation.axisAt(axisNum).zeroValue());

        for (int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
            int ledNum = INITIAL_LED;
            for(int i = 0; i < iAnimation.numLeds(); i++) {
                while(iAnimation.isMissing(ledNum)) {
                    qDebug("led %d is missing", ledNum);
                    ledNum++;
                }

                if(iWriteLedNumber) {
                    writeCharacter(ledNum);
                }

                AxisData* axisData = NULL;

                if(axisNum == kTimeAxisNum) {
                    axisData = iAnimation.ledAt(ledNum)->timeAxis();
                } else {
                    axisData = &iAnimation.ledAt(ledNum)->axisAt(axisNum);
                }

                writeCharacter(axisData->frameAt(frame).value().type());

                switch(axisData->frameAt(frame).value().type()) {
                case kColour:
                    writeColour(axisData->frameAt(frame));
                    break;
                case kFunction:
                    writeFunction(axisData->frameAt(frame));
                    break;
                }

                ledNum++;
            }
        }
    }

}

void LedAnimCodec::writeAxisData(int axisNum) {

    Axis* axis = iAnimation.timeAxis();

    for (int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
        int ledNum = INITIAL_LED;
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            while(iAnimation.isMissing(ledNum)) {
                qDebug("led %d is missing", ledNum);
                ledNum++;
            }

            if(iWriteLedNumber) {
                writeCharacter(ledNum);
            }

            AxisData* axisData = NULL;

          //  if(axisNum == kTimeAxisNum) {
                axisData = iAnimation.ledAt(ledNum)->timeAxis();
          //  } else {
                //axisData = &iAnimation.ledAt(ledNum)->axisAt(axisNum);
          //  }

            writeCharacter(axisData->frameAt(frame).value().type());

            switch(axisData->frameAt(frame).value().type()) {
            case kColour:
                writeColour(axisData->frameAt(frame));
                break;
            case kFunction:
                writeFunction(axisData->frameAt(frame));
                break;
            }

            ledNum++;
        }
    }
}

void LedAnimCodec::readAxis(int axisNum) {
    Axis* axis = NULL;

    int axisType = readCharacter().intValue();
    int priority = readCharacter().intValue();
    bool opaque = readCharacter().boolValue();

    if(axisNum == kTimeAxisNum) {
        int lowValue = readCharacter().intValue();
        int highValue = readCharacter().intValue();
        int speed = readCharacter().intValue();

        iAnimation.addTimeAxis(lowValue, highValue, speed, priority, opaque);
       // axis = iAnimation.timeAxis();
    } else {
        int lowValue = readCharacter().intValue();
        int highValue = readCharacter().intValue();
        int zeroValue = readCharacter().intValue();

        iAnimation.addValueAxis(lowValue, highValue, zeroValue, priority, opaque);
        axis = &iAnimation.axisAt(axisNum);

        for(int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
            int ledNum = INITIAL_LED;
            for(int i = 0; i < iAnimation.numLeds(); i++) {
                readCharacter();
                while(iAnimation.isMissing(ledNum)){
                    ledNum++;
                }

                Led* led = iAnimation.ledAt(ledNum++);

                AxisData* axisData = NULL;

                if(axisNum == kTimeAxisNum) {
                    axisData = led->timeAxis();
                } else {
                    axisData = &led->axisAt(axisNum);
                }

                char frameType = readCharacter().unsignedCharValue();

                switch(frameType) {
                case kColour:
                    axisData->frameAt(frame).doSetValue(*(new ColourValue(led, readColour())));
                    break;
                case kFunction:
                    Frame& newFrame = axisData->frameAt(frame);
                    newFrame.doSetValue(readFunction(newFrame));
                    break;
                }


            }
        }
    }
}

void LedAnimCodec::readAxisData(int axisNum) {


    Axis* axis = iAnimation.timeAxis();

    if(axis != NULL) {

    for(int frame = axis->lowValue(); frame <= axis->highValue(); frame++) {
        int ledNum = INITIAL_LED;
        for(int i = 0; i < iAnimation.numLeds(); i++) {
            readCharacter();
            while(iAnimation.isMissing(ledNum)){
                ledNum++;
            }

            Led* led = iAnimation.ledAt(ledNum++);

            AxisData* axisData = NULL;

            //if(axisNum == kTimeAxisNum) {
                axisData = led->timeAxis();
           // } else {
           //     axisData = &led->axisAt(axisNum);
           // }

            char frameType = readCharacter().unsignedCharValue();

            switch(frameType) {
            case kColour:
                axisData->frameAt(frame).doSetValue(*(new ColourValue(led, readColour())));
                break;
            case kFunction:
                Frame& newFrame = axisData->frameAt(frame);
                newFrame.doSetValue(readFunction(newFrame));
                break;
            }


        }
    }
    }
}

// -----------------------------------------

LedAnimStringCodec::LedAnimStringCodec(Animation &animation) :
    LedAnimCodec(animation),
    iLastReturn(0) {
}

const AnimChar LedAnimStringCodec::readCharacter() const {
    return AnimChar();
}

void LedAnimStringCodec::writeCharacter(AnimChar character) {
    unsigned char value = character.unsignedCharValue();

    if(value == TERMINATING_BYTE ||
       value == ESCAPE_BYTE ||
       value == HEADER_BYTE ) {
        iString.append(QString::number(ESCAPE_BYTE));
        iString.append(",");
        iString.append(QString::number((value ^ XOR_BYTE)));
    } else {
        iString.append(QString::number(value));
    }

    iString.append(",");

    int extra = iString.length() - iLastReturn;
    if(extra > 100) {
        iString.append("\n");
        iLastReturn = iString.length();
    }
}

void LedAnimStringCodec::writeControlCharacter(AnimChar character) {
    int value = character.intValue();

    if(value != TERMINATING_BYTE &&
       value != HEADER_BYTE ) {
        throw IllegalArgumentException("LedAnimStringCodec::writeControlCharacter : not a control character");
    }

    iString.append(QString::number(character.intValue()));

    if(value == HEADER_BYTE) {
        iString.append(",");
    }
}

void LedAnimStringCodec::writeColour(Frame &frame) {
    const ColourValue& colourValue = static_cast<const ColourValue&>(frame.value());

    writeCharacter(colourValue.colour().red());
    writeCharacter(colourValue.colour().green());
    writeCharacter(colourValue.colour().blue());
}

const QColor LedAnimStringCodec::readColour() const {

}

void LedAnimStringCodec::writeFunction(Frame& frame) {
    const FunctionValue& value = static_cast<const FunctionValue&>(frame.value());

    writeCharacter(value.number());
}

const FunctionValue& LedAnimStringCodec::readFunction(Frame& frame) const {

}

void LedAnimStringCodec::writeFunctionData(Function function) {
    float multiplier = 256 * 256;

    float redIncrementF = (float)function.redIncrement();
    double redIncrementD = (double)redIncrementF;
    redIncrementD *= multiplier;
    redIncrementD = round(redIncrementD) - 1;
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
    greenIncrementD = round(greenIncrementD) - 1;
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
    blueIncrementD = round(blueIncrementD) - 1;
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

Function LedAnimStringCodec::readFunctionData() const {
    return Function();
}


const QString LedAnimStringCodec::asString() const {
    return iString;
}

const QByteArray& LedAnimStringCodec::asByteArray() const {
    return QByteArray();
}

// -----------------------------------------

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
    if(iByteArray.at(iPosition) == ESCAPE_BYTE) {
        AnimChar character(iByteArray.at(iPosition+1) ^ XOR_BYTE);
        iPosition = iPosition + 2;
        return character;
    } else {
        return AnimChar(iByteArray.at(iPosition++));
    }
}

void LedAnimByteArrayCodec::writeCharacter(AnimChar character) {
    char value = character.unsignedCharValue();

    if(value == TERMINATING_BYTE ||
       value == ESCAPE_BYTE ||
       value == HEADER_BYTE ) {
        iByteArray.append(ESCAPE_BYTE);
        iByteArray.append(value ^ XOR_BYTE);
    } else {
        iByteArray.append(value);
    }
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

        writeCharacter(ledNum++);
        writeCharacter(position.row());
        writeCharacter(position.column());
    }
}

QList<int> LedAnimByteArrayCodec::readPositionData(int* numRows, int* numColumns, int numLeds) {
    QList<int> positions;

    int rows = readCharacter().intValue();
    int columns = readCharacter().intValue();

    for(int i = 0; i < rows * columns; i++) {
        positions.append(INVALID);
    }

    int ledNum;
    int row;
    int column;

    for(int i = 0; i < numLeds; i++) {
        ledNum = readCharacter().intValue();
        row = readCharacter().intValue();
        column = readCharacter().intValue();

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

    int hue = colourValue.colour().hue();
    char hueHigh = hue;
    char hueLow = hue >> 8;
    writeCharacter(hueHigh);
    writeCharacter(hueLow);
    writeCharacter(colourValue.colour().saturation());
    writeCharacter(colourValue.colour().value());
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

    writeCharacter(colourValue.colour().red());
    writeCharacter(colourValue.colour().green());
    writeCharacter(colourValue.colour().blue());
}

const QColor LedAnimSimpleByteArrayCodec::readColour() const {

}

void LedAnimSimpleByteArrayCodec::writeFunction(Frame& frame) {
    const FunctionValue& value = static_cast<const FunctionValue&>(frame.value());

    writeCharacter(value.number());
}

const FunctionValue& LedAnimSimpleByteArrayCodec::readFunction(Frame& frame) const {

}

void LedAnimSimpleByteArrayCodec::writeFunctionData(Function function) {   
    float multiplier = 256 * 256;

    float redIncrementF = (float)function.redIncrement();
    double redIncrementD = (double)redIncrementF;
    redIncrementD *= multiplier;
    redIncrementD = round(redIncrementD);
    int redIncrement = (int)redIncrementD;

    char redIncrementHigh = redIncrement;
    char redIncrementLow = redIncrement >> 8;
    writeCharacter(redIncrementHigh);
    writeCharacter(redIncrementLow);

    float greenIncrementF = (float)function.greenIncrement();
    double greenIncrementD = (double)greenIncrementF;
    greenIncrementD *= multiplier;
    greenIncrementD = round(greenIncrementD);
    int greenIncrement = (int)greenIncrementD;

    char greenIncrementHigh = greenIncrement;
    char greenIncrementLow = greenIncrement >> 8;
    writeCharacter(greenIncrementHigh);
    writeCharacter(greenIncrementLow);

    float blueIncrementF = (float)function.blueIncrement();
    double blueIncrementD = (double)blueIncrementF;
    blueIncrementD *= multiplier;
    blueIncrementD = round(blueIncrementD);
    int blueIncrement = (int)blueIncrementD;

    char blueIncrementHigh = blueIncrement;
    char blueIncrementLow = blueIncrement >> 8;
    writeCharacter(blueIncrementHigh);
    writeCharacter(blueIncrementLow);
}

Function LedAnimSimpleByteArrayCodec::readFunctionData() const {
    return Function();
}

