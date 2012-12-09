#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class Exception {

public:
    inline Exception (QString errorMessage) { iErrorMessage = errorMessage; }

private:
    QString iErrorMessage;
};

class InvalidStateException : public Exception {

public:
    inline InvalidStateException(QString errorMessage) : Exception (errorMessage) {}
};

class InvalidAnimationException  : public Exception {

public:
    inline InvalidAnimationException(QString errorMessage) : Exception (errorMessage) {}

private:
    QString iErrorMessage;
};

#endif // EXCEPTIONS_H
