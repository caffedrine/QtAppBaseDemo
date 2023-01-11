#include "QTreeWidgetExtended.h"
#include <QEvent>
#include <QKeyEvent>

QTreeWidgetExtended::QTreeWidgetExtended(QWidget *parent): QTreeWidget(parent)
{
    // install the event filter
    this->installEventFilter(this);
}

bool QTreeWidgetExtended::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Delete)
        {
            QTreeWidgetItem* item = this->currentItem();
            if (item) {
                delete item;
            }
            return true;
        }
    }
    return QTreeWidget::eventFilter(object, event);
}
