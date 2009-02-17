#include <QtGui/QApplication>
#include "falcontestqt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FalconTestQt w;
    w.show();
    return a.exec();
}
