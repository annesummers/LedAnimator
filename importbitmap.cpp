#include "importbitmap.h"

#include <QImage>

#include "animation.h"
#include "led.h"

using namespace ImportExport;

ImportBitmap::ImportBitmap(QString fileName, Animation& animation) :
    QObject(NULL),
    iAnimation(animation),
    iFileName(fileName) {
}

void ImportBitmap::doImport() {
    QImage image(iFileName);

    // check for max height and width of animation

    int rows = image.height();
    int columns = image.width() ;

    QList<int> positions;
    QList<QRgb> colours;
    int ledNum = 0;

    for(int row = 0; row < rows; row++) {
        for(int column = 0; column < columns; column++) {
            positions.append(INVALID);

            QRgb color = image.pixel(column, row);
            if(qAlpha(color) != 0) {
                positions.replace(row * columns + column, ++ledNum);
                colours.append(color);
            }
        }
    }

    iAnimation.setupNew(rows, columns, 100, 100, ledNum, positions);

    ledNum = INITIAL_LED;
    for(int i = 0; i < iAnimation.numLeds(); i++) {
        for(int j = 0; j < iAnimation.numFrames(); j++) {
            iAnimation.ledAt(ledNum)->frameAt(j + INITIAL_FRAME).doSetColour(QColor(colours[i]));
        }

        ledNum++;
    }
}
