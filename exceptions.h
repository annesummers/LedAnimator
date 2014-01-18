/*****************************************
**                                      **
** Copyright (C) 2012-2013 Anne Summers **
**                                      **
*****************************************/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>
#include <QtCore>
//#include <QtConcurrent/QtConcurrent>

namespace Exception {

class AnimatorException : public QException {

public:
    inline AnimatorException (QString errorMessage) { iErrorMessage = errorMessage; }

    void raise() const { throw *this; }
    QException *clone() const { return new AnimatorException(*this); }

    inline QString errorMessage() { return iErrorMessage; }

    ~AnimatorException() throw() {  }

private:
    QString iErrorMessage;
};

class InvalidStateException : public AnimatorException {

public:
    inline InvalidStateException(QString errorMessage) : AnimatorException (errorMessage) {}
};


class InvalidFrameException : public AnimatorException {

public:
    inline InvalidFrameException(QString errorMessage) : AnimatorException (errorMessage) {}
};

class InvalidAnimationException  : public AnimatorException {

public:
    inline InvalidAnimationException(QString errorMessage) : AnimatorException (errorMessage) {}
};

class InvalidPositionException  : public AnimatorException {

public:
    inline InvalidPositionException(QString errorMessage) : AnimatorException (errorMessage) {}
};

class IllegalArgumentException : public AnimatorException {

public:
    inline IllegalArgumentException(QString errorMessage) : AnimatorException (errorMessage) {}
};

class IllegalColourException : public AnimatorException {

public:
    inline IllegalColourException() : AnimatorException ("Invalid colour") {}
};

class IllegalFrameNumberException : public AnimatorException {

public:
    inline IllegalFrameNumberException(QString errorMessage) : AnimatorException (errorMessage) {}

};

class IllegalAxisNumberException : public AnimatorException {

public:
    inline IllegalAxisNumberException(QString errorMessage) : AnimatorException (errorMessage) {}

};

class IllegalStateException : public AnimatorException {

public:
    inline IllegalStateException(QString errorMessage) : AnimatorException (errorMessage) {}

};

class NotImplementedException : public AnimatorException {

public:
    inline NotImplementedException(QString errorMessage) : AnimatorException (errorMessage) {}

};
}

#endif // EXCEPTIONS_H
