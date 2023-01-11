#include "QDialogExtended.h"

QDialogExtended::QDialogExtended(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    this->setWindowFlags(Qt::Window);
}

QDialogExtended::~QDialogExtended()
{

}
