#ifndef LEDANIMCODEC_H
#define LEDANIMCODEC_H

#include <QObject>
#include <QList>
#include <QPoint>
#include <QColor>
#include <QSize>

class Animation;
class Led;

class AnimChar {
public:
    AnimChar();
    AnimChar(int value);
    AnimChar(unsigned char value);

    int intValue();
    unsigned char charValue();

private:
    unsigned char iValue;
};

class AnimStringChar : public AnimChar {

};

class AnimByteChar : public AnimChar {

};

class LedAnimCodec : public QObject
{
    Q_OBJECT
public:
    explicit LedAnimCodec(Animation &animation);

    void writeAnimation();
    void readAnimation();

signals:
    
public slots:

protected:
    virtual AnimChar readCharacter() = 0;
    virtual void writeCharacter(AnimChar character) = 0;

    virtual QByteArray&  asByteArray() = 0;
    virtual QString     asString() = 0;

private:
    void writeColour(QColor colour);
    QColor readColour();

    Animation& iAnimation;
};

class LedAnimStringCodec: public LedAnimCodec {
    Q_OBJECT
public:
    explicit LedAnimStringCodec(Animation &animation);

protected:
    AnimChar readCharacter();
    void writeCharacter(AnimChar character);

    QString asString();
    QByteArray& asByteArray();

private:
    QString iString;
};

class LedAnimByteArrayCodec: public LedAnimCodec {
    Q_OBJECT
public:
    explicit LedAnimByteArrayCodec(Animation& animation);
    LedAnimByteArrayCodec(Animation& animation, QByteArray bytes);


    QString asString();
    QByteArray& asByteArray();

protected:
    AnimChar readCharacter();
    void writeCharacter(AnimChar character);

private:
    QByteArray iByteArray;
    int         iPosition;
};

#endif // LEDANIMCODEC_H
