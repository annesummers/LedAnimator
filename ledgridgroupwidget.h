#ifndef LEDGRIDGROUPWIDGET_H
#define LEDGRIDGROUPWIDGET_H

#include <QWidget>

#include "colourgroupgroupwidget.h"

namespace AnimatorUi {

class LedGridGroupWidget : public ColourGroupGroupWidget {
    Q_OBJECT

public:
    explicit LedGridGroupWidget(QWidget *parent = 0);
    
private:
    inline QString mimeType() const { return LED_MIME_TYPE; }
    
};
}

#endif // LEDGRIDGROUPWIDGET_H
