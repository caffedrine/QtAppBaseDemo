#ifndef ADVANCEDTABLEWIDGET_H
#define ADVANCEDTABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QHeaderView>
#include <QDebug>
#include <QClipboard>
#include <QApplication>
#include <QSaveFile>

#include "utils/utils.h"

class AdvancedTableWidget: public QTableWidget
{
    Q_OBJECT
public:
    explicit AdvancedTableWidget(QWidget *parent = nullptr);
    explicit AdvancedTableWidget(int rows, int columns, QWidget *parent = nullptr);

    void ExportAsCSV(QString outputFile, QList<int> columnsToBeExported = QList<int>());

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

signals:
    void OnDraggedFile(QString filename);
    void OnTextPasted(QString pastedText);
    void OnRowsCopy(QModelIndexList selectedRows);
    void OnDoubleClickWithoutSelection();

private:
        void AdvancedTableWidgetInit();
        void keyPressEvent(QKeyEvent *event);
        void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // ADVANCEDTABLEWIDGET_H
