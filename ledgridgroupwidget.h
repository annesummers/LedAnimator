#ifndef LEDGRIDGROUPWIDGET_H
#define LEDGRIDGROUPWIDGET_H

#include <QWidget>

#include "colourgroupgroupwidget.h"

namespace AnimatorUi {

class LedGridGroupWidget : public ColourGroupGroupWidget {
    Q_OBJECT

public:
    explicit LedGridGroupWidget(QWidget *parent, MainWindow& mainWindow);
    
private:
    inline QString mimeType() const { return LED_MIME_TYPE; }
    inline bool canCut() const { return true; }
    
};
}

#endif // LEDGRIDGROUPWIDGET_H
