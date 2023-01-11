#include "QTabWidgetExtended.h"
#include "QTabBarExtended.h"
#include <QMouseEvent>

QTabWidgetExtended::QTabWidgetExtended(QWidget *parent) : QTabWidget(parent)
{
    // Set a custom bar
    this->setTabBar(new QTabBarExtended(this));

    QObject::connect(this->tabBar(), SIGNAL(NewTabRequested()), this, SIGNAL(NewTabRequested()));

    this->setTabsClosable(true);

    // Remove borders
    //this->setStyleSheet(QString("QTabWidget::pane { border-left: 0; border-bottom: 0; border-right: 0; border-top: 2px;}"));

}