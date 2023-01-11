#include "DictTreeModel.h"

//
// TREE ITEM
//

DictTreeItem::DictTreeItem(const QList<QVariant> &data, DictTreeItem *parent): m_itemData(data), m_parentItem(parent)
{

}

DictTreeItem::~DictTreeItem()
{
    qDeleteAll(m_childItems);
}

void DictTreeItem::appendChild(DictTreeItem *item)
{
    m_childItems.append(item);
}

DictTreeItem *DictTreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int DictTreeItem::childCount() const
{
    return m_childItems.count();
}

int DictTreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant DictTreeItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return {};
    return m_itemData.at(column);
}

DictTreeItem *DictTreeItem::parentItem()
{
    return m_parentItem;
}

int DictTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<DictTreeItem*>(this));

    return 0;
}

//
// TREE MODEL
//

DictTreeModel::DictTreeModel(Dict *dict, QObject *parent): QAbstractItemModel(parent)
{
    rootItem = new DictTreeItem({tr("Key"), tr("Val")});
    setupModelData(dict, rootItem);
}

DictTreeModel::~DictTreeModel()
{
    delete rootItem;
}

int DictTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<DictTreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

QVariant DictTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DictTreeItem *item = static_cast<DictTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags DictTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant DictTreeModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex DictTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DictTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DictTreeItem*>(parent.internalPointer());

    DictTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex DictTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    DictTreeItem *childItem = static_cast<DictTreeItem*>(index.internalPointer());
    DictTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DictTreeModel::rowCount(const QModelIndex &parent) const
{
    DictTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DictTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void DictTreeModel::setupModelData(Dict *dict, DictTreeItem *parent) // NOLINT(readability-convert-member-functions-to-static,misc-no-recursion)
{
    QMapIterator<QString, Dict *> it(*dict->dataPtr());
    while (it.hasNext())
    {
        it.next();

        //DictTreeItem *curr = new DictTreeItem({{(!it.value()->val.isNull() ? it.value()->val.toString() : "null")}}, parent);
        DictTreeItem *curr = new DictTreeItem({{it.key(), (!it.value()->val.isNull() ? it.value()->val.toString() : "null")}}, parent);

        parent->appendChild( curr );

        // Print next children from this dict
        setupModelData(it.value(), curr); // value() = QDict
    }
}