#ifndef COLOURGROUPGROUPWIDGET_H
#define COLOURGROUPGROUPWIDGET_H

#include <QWidget>

#include "selectablegroupgroupwidget.h"

namespace AnimatorUi {

class ColourGroupGroupWidget : public SelectableGroupGroupWidget {
    Q_OBJECT

public:
    explicit ColourGroupGroupWidget(QWidget *parent, Engine &engine);

    void setColour(QColor colour);
    
signals:
    
public slots:
    
};
}

#endif // COLOURGROUPGROUPWIDGET_H
