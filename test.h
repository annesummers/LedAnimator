/*************************************
**                                  **
** Copyright (C) 2012 Anne Summers  **
**                                  **
**************************************/

#ifndef TEST_H
#define TEST_H

/*#include <QList>
#include <QObject>

class Test {
public:
    //static
    static Test* get();

    int runAll(int argc, char **argv);
    void addTestObject(QObject *o);

private:
    Test();
    ~Test();

    QList<QObject*> m_testObjects;
};


template <class T>
class TestRegistar {
  public:
    inline TestRegistar(const char *name) {
        T *test = new T;
        test->setObjectName(name);
        Test::get()->addTestObject(test);
    }
};

#define TEST(T) \
    static ::TestRegistar<T> _test_registar_##T(""#T);*/

#endif // TEST_H
