#include "falcontestqt.h"
#include "ui_falcontestqt.h"

FalconTestQt::FalconTestQt(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::FalconTestQtClass)
{
    ui->setupUi(this);
}

FalconTestQt::~FalconTestQt()
{
    delete ui;
}
