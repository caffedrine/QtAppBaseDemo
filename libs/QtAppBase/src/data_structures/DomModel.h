#ifndef CRYPTOPOCKETKNIFE_DOMMODEL_H
#define CRYPTOPOCKETKNIFE_DOMMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QDomDocument>


class DomItem
{
public:
    DomItem(const QDomNode &node, int row, DomItem *parent = nullptr);
    ~DomItem();
    DomItem *child(int i);
    DomItem *parent();
    QDomNode node() const;
    int row() const;

private:
    QDomNode domNode;
    QHash<int, DomItem *> childItems;
    DomItem *parentItem;
    int rowNumber;
};

class DomModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit DomModel(QObject *parent = nullptr);
    explicit DomModel(const QDomDocument &document, QObject *parent = nullptr);
    ~DomModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool loadDom(const QDomDocument &document);
    void clear();
private:
    QDomDocument domDocument;
    DomItem *rootItem = nullptr;
};


#endif //CRYPTOPOCKETKNIFE_DOMMODEL_H
