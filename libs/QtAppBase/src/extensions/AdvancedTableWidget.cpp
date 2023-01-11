#include "AdvancedTableWidget.h"

AdvancedTableWidget::AdvancedTableWidget(QWidget *parent): QTableWidget(parent)
{
    this->AdvancedTableWidgetInit();
}

AdvancedTableWidget::AdvancedTableWidget(int rows, int columns, QWidget *parent): QTableWidget(rows, columns, parent)
{
    this->AdvancedTableWidgetInit();
}

void AdvancedTableWidget::AdvancedTableWidgetInit()
{
    this->setAcceptDrops(true);

    this->verticalHeader()->setVisible(false);
    this->setSortingEnabled(true);
    this->setSelectionBehavior(SelectionBehavior::SelectRows);
    this->installEventFilter(this);

    QString styleSheet = "::section "
                         "{"
                                "background-color: #edf0f5;"
                         "}";
    this->horizontalHeader()->setStyleSheet(styleSheet);

}

void AdvancedTableWidget::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

void AdvancedTableWidget::dropEvent(QDropEvent *e)
{
    for (const QUrl &url: e->mimeData()->urls())
    {
        QString fileName = url.toLocalFile();
        emit OnDraggedFile(fileName);
    }
}

void AdvancedTableWidget::keyPressEvent(QKeyEvent *event)
{
    QModelIndexList selectedRows = this->selectionModel()->selectedRows();

    // Delete selected row
    if(event->key() == Qt::Key_Delete)
    {
        if( !selectedRows.isEmpty() )
        {
            model()->removeRows(selectedRows.at(0).row(), selectedRows.size());
        }
    }

    // Copy selected row
    else if(event->matches(QKeySequence::Copy))
    {
        if(!selectedIndexes().isEmpty())
        {
            emit OnRowsCopy(selectedRows);
//            QString text;
//            QItemSelectionRange range = selectionModel()->selection().first();
//            for (auto i = range.top(); i <= range.bottom(); ++i)
//            {
//                QStringList rowContents;
//                for (auto j = range.left(); j <= range.right(); ++j)
//                    rowContents << model()->index(i,j).data().toString();
//                text += rowContents.join("\t");
//                text += "\n";
//            }
//            QApplication::clipboard()->setText(text);
        }
    }

    // Paste from clipboard
    else if(event->matches(QKeySequence::Paste))
    {
        emit this->OnTextPasted( QApplication::clipboard()->text());

//        QString text = QApplication::clipboard()->text();
//        QStringList rowContents = text.split("\n", QString::SkipEmptyParts);

//        QModelIndex initIndex = selectedIndexes().at(0);
//        auto initRow = initIndex.row();
//        auto initCol = initIndex.column();

//        for (auto i = 0; i < rowContents.size(); ++i)
//        {
//            QStringList columnContents = rowContents.at(i).split("\t");
//            for (auto j = 0; j < columnContents.size(); ++j)
//            {
//                model()->setData(model()->index(initRow + i, initCol + j), columnContents.at(j));
//            }
//        }
    }

    // Send to default handler
    else
    {
        QTableView::keyPressEvent(event);
    }
}

void AdvancedTableWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndexList selectedRows = this->selectionModel()->selectedRows();

    if( selectedRows.count() <= 0 )
    {
        this->OnDoubleClickWithoutSelection();
        return;
    }

    // Forward event to regular handler
    QTableView::mouseDoubleClickEvent(event);
}

void AdvancedTableWidget::ExportAsCSV(QString outputFile, QList<int> columnsToBeExported)
{
    if( outputFile.isEmpty() )
    {
        qDebug() << "Invalid file name provided while trying to export table to CSV file.";
        return;
    }

    // Check if table have any columns
    if( columnCount() <= 0 )
    {
        qDebug() << "No columns to be exported within the table";
        return;
    }

    // Validate columns range given
    for( const int i: columnsToBeExported )
    {
        if( i >=  this->columnCount())
        {
            qDebug() << "Wrong number of columns provided";
            return;
        }
    }

    // If no columns are provided, export the entire table, which mean all cols shall be set
    if( columnsToBeExported.count() == 0 )
    {
        for( int i = 0; i < this->columnCount(); i++ )
        {
            columnsToBeExported.append(i);
        }
    }

    // Open file for writing
    QSaveFile file(outputFile);
    file.open(QIODevice::WriteOnly);

    // Write header to file
    for( int i = 0; i < columnsToBeExported.count() - 1; i++ )
    {
        file.write(this->horizontalHeaderItem(columnsToBeExported.at(i))->text().toUtf8() + ",");
    }
    file.write(this->horizontalHeaderItem(columnsToBeExported.at(columnsToBeExported.last()))->text().toUtf8() + "\n");

    // Write rows to file
    for( int i = 0; i < this->rowCount(); i++)
    {
        QString line = "";
        for( int j = 0; j < columnsToBeExported.count() - 1; j++)
        {
            line += Util_EncodeForCSV( this->item(i, columnsToBeExported.at(j))->text()) + ",";
        }
        line += Util_EncodeForCSV( this->item(i, columnsToBeExported.at(columnsToBeExported.last()))->text() );
        line += "\n";
        file.write(line.toUtf8());
    }

    // Commit changes to file
    file.commit();
}
