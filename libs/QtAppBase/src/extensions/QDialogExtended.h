/*  
 *  File      : QDialogExtended.h
 *  Created on: 2022-04-12
 *      Author: uib74520
 */

#ifndef _QDIALOGEXTENDED_H_
#define _QDIALOGEXTENDED_H_

#include <QDialog>

class QDialogExtended: public QDialog
{
    Q_OBJECT
public:
    QDialogExtended(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    ~QDialogExtended();

protected:

private:

};

#endif // _QDIALOGEXTENDED_H_