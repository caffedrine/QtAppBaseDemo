// TEST:
//#include "Dict.h"
//#include "DictTreeModel.h"
//#include <QTreeView>
//
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    qDebug().noquote().nospace() << "Started app...";
//
//    Dict d;
//    d["a"] = QVariant("1");
//    d["a"]["b"] = QVariant("2");
//    d["a"]["b"]["c"] = QVariant("3");
//    d["a"]["b"]["d"] = QVariant("4");
//    d["a"]["b"]["e"] = QVariant("5");
//    d["a"]["b"]["e"]["f"] = QVariant("6");
//    d["a"]["x"] = QVariant("7");
//    d["a"]["y"]["z"] = QVariant("8");
//    d["m"] = QVariant("11");
//    d["m"]["m"] = QVariant("12");
//    d["m"]["n"]["t"] = QVariant("13");
//    d["x"] = QVariant("9");
//    d["y"]["z"] = QVariant("10");
//
//    qDebug().noquote().nospace() << d;
//    qDebug().noquote().nospace() << d.toString(true);
//
//    QTreeView view;
//    DictTreeModel model(&d);
//    view.setModel(&model);
//    view.setWindowTitle(QObject::tr("Simple Tree Model"));
//    view.show();
//
//    return QApplication::exec();
//}

#ifndef _DICT_H_
#define _DICT_H_

#include <QString>
#include <QVariant>
#include <QMap>

class Dict
{
public:
    QVariant val;

    explicit Dict() = default;
    explicit Dict(QVariant val): val(std::move(val)) { };
    ~Dict();

    Dict& operator[](const QString &idx);
    Dict& operator=(const QVariant &v);
    virtual operator QString(); // NOLINT(google-explicit-constructor)

    void unset(const QString &idx);
    bool isset(const QString &idx);
    void set(const QString &idx, const QVariant &value = QVariant());
    Dict *get(const QString &idx);
    QMap<QString, Dict *> *dataPtr();
    QString toString(bool tree_view = false) const; // NOLINT(modernize-use-nodiscard)

private:
    QMap<QString, Dict *> map;
    QString buildString(const Dict *next, const QString& parent = "") const;
    QString buildStringTree(const Dict *next, int level = 0) const;
};

#endif //_DICT_H_
