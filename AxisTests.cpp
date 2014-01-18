#include "AxisTests.h"

#include "Animation.h"
#include "exceptions.h"

using namespace AnimatorTest;
using namespace Exception;

AxisTests::AxisTests(QObject *parent) :
    QObject(parent)

{
}

void AxisTests::initTestCase(){
}



void AxisTests::setCurrentFrame_data() {
    QTest::addColumn<int>("currentFrame");
    QTest::addColumn<QString>("errorString");

    QTest::newRow("frame number too small") << 0
                                       << "Animation::setCurrentFrame : Frame number is smaller than first frame";
    //QTest::newRow("frame number too big") << iAnimation->numFrames() + 1
    //                                      << "Animation::setCurrentFrame : Frame number is greater than last frame";
 //   QTest::newRow("valid") << INITIAL_FRAME
  //                         << "";
}

void AxisTests::setCurrentFrame() {
    QFETCH(int, currentFrame);
    QFETCH(QString, errorString);

    try {
        iAxis->setCurrentFrame(currentFrame);
    } catch(IllegalArgumentException& e) {
        QCOMPARE(e.errorMessage(), errorString);
    }
}


void AxisTests::cleanupTestCase(){
}
