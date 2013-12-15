#ifndef COLOURGROUPGROUPWIDGET_H
#define COLOURGROUPGROUPWIDGET_H

#include <QWidget>

#include "selectablegroupgroupwidget.h"

namespace AnimatorModel {
class FrameValue;
}

namespace AnimatorUi {

class ColourGroupGroupWidget : public SelectableGroupGroupWidget {
    Q_OBJECT

public:
    explicit ColourGroupGroupWidget(QWidget *parent, Engine &engine);

    void setValue(FrameValue& value);
    
signals:
    
public slots:
    
};
}

#endif // COLOURGROUPGROUPWIDGET_H
