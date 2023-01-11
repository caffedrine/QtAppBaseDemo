#ifndef _TREEMODEL_H
#define _TREEMODEL_H

#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"

#include <QObject>
#include <QVariant>
#include <QAbstractItemModel>
#include <QStringList>
#include "Dict.h"

class DictTreeItem
{
public:
    explicit DictTreeItem(const QList<QVariant> &data, DictTreeItem *parentItem = nullptr);
    ~DictTreeItem();

    void appendChild(DictTreeItem *child);

    DictTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    DictTreeItem *parentItem();

private:
    QList<DictTreeItem *> m_childItems;
    QList<QVariant> m_itemData;
    DictTreeItem *m_parentItem;
};

class DictTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DictTreeModel(Dict *dict, QObject *parent = nullptr);
    ~DictTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    void setupModelData(Dict *dict, DictTreeItem *parent);
    DictTreeItem *rootItem;
};

#pragma clang diagnostic pop

#endif //_TREEMODEL_H