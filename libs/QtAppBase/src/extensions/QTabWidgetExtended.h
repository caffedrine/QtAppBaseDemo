#ifndef _QTABWIDGETEXTENDED_H_
#define _QTABWIDGETEXTENDED_H_

#include <QTabWidget>

class QTabWidgetExtended: public QTabWidget
{
    Q_OBJECT
public:
    explicit QTabWidgetExtended(QWidget *parent = nullptr);

signals:
    void NewTabRequested();
};


#endif //_QTABWIDGETEXTENDED_H_
