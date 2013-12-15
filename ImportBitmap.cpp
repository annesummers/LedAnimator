#include "importbitmap.h"

#include <QImage>

#include "Animation.h"
#include "Led.h"
#include "ColourValue.h"

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

    iAnimation.newAnimation(rows, columns, ledNum, positions);
    iAnimation.addTimeAxis(0, 1, 100);

    ledNum = INITIAL_LED;
    for(int i = 0; i < iAnimation.numLeds(); i++) {
        for(int j = iAnimation.timeAxis()->lowValue(); j < iAnimation.timeAxis()->highValue(); j++) {
            Frame& frame = iAnimation.ledAt(ledNum)->timeAxis()->frameAt(j);
            frame.doSetValue(*(new ColourValue(iAnimation.ledAt(ledNum), QColor(colours[i]))));
        }

        ledNum++;
    }
}
