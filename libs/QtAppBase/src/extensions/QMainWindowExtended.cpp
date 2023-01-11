#include "QMainWindowExtended.h"

QMainWindowExtended::QMainWindowExtended(QWidget *parent): QMainWindow(parent)
{

}

QMainWindowExtended::~QMainWindowExtended()
{

}

void QMainWindowExtended::exec()
{
    this->show();
    QEventLoop loop;
    connect(this, SIGNAL(destroyed()), &loop, SLOT(quit()));
    loop.exec();
}

