#include "QListViewModel.h"

QListViewModel::QListViewModel(const QStringList &lines, QObject *parent): QAbstractListModel(parent), mLines(lines)
{
}

int QListViewModel::rowCount(const QModelIndex &parent) const
{
    // Return the number of lines of text
    return mLines.size();
}

QVariant QListViewModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mLines.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return mLines[index.row()];
    else
        return QVariant();
}
