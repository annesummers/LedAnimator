#ifndef IMPORTBITMAP_H
#define IMPORTBITMAP_H

#include <QObject>

namespace AnimatorModel {
class Animation;
}

using namespace AnimatorModel;

namespace ImportExport {

class ImportBitmap : public QObject {
    Q_OBJECT

public:
    explicit ImportBitmap(QString fileName, Animation &animation);

    void doImport();
    
signals:
    
public slots:

private:
    Animation& iAnimation;
    QString iFileName;
    
};
}

#endif // IMPORTBITMAP_H
