#ifndef FALCONTESTQT_H
#define FALCONTESTQT_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class FalconTestQtClass;
}

class FalconTestQt : public QMainWindow
{
    Q_OBJECT

public:
    FalconTestQt(QWidget *parent = 0);
    ~FalconTestQt();

private:
    Ui::FalconTestQtClass *ui;
};

#endif // FALCONTESTQT_H
