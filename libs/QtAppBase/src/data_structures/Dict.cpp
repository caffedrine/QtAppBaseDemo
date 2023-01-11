#include "Dict.h"

Dict::~Dict()
{
    QMapIterator<QString, Dict *> i(this->map);
    while (i.hasNext())
    {
        i.next();
        delete i.value();
    }
}

Dict &Dict::operator[](const QString &idx)
{
    if( this->map.find(idx) == this->map.end() )
    {
        this->map.insert(idx, new Dict());
    }
    return (*this->map[idx]);
}

Dict &Dict::operator=(const QVariant &v)
{
    this->val = v;
    return *this;
}

Dict::operator QString()
{
    return this->toString();
}

void Dict::unset(const QString &idx)
{
    // clean up memory
    delete this->map.value(idx);

    if( this->map.find(idx) != this->map.end() )
        this->map.remove(idx);
}

bool Dict::isset(const QString &idx)
{
    return this->map.find(idx) != this->map.end();
}

void Dict::set(const QString &idx, const QVariant &v)
{
    if( this->map.find(idx) == this->map.end() )
    {
        this->map.insert(idx, new Dict(v));
    }
    else
    {
        this->map[idx]->val = v;
    }
}

Dict *Dict::get(const QString &idx)
{
    return this->map[idx];
}

QMap<QString, Dict *> *Dict::dataPtr()
{
    return &this->map;
}

QString Dict::toString(bool tree_view) const // NOLINT(readability-convert-member-functions-to-static,modernize-use-nodiscard,misc-no-recursion)
{
    if( tree_view )
        return this->buildStringTree(this);
    return this->buildString(this);
}

QString Dict::buildString(const Dict *next, const QString &parent) const // NOLINT(misc-no-recursion)
{
    QString output = "";
    QMapIterator<QString, Dict *> it(next->map);
    while (it.hasNext())
    {
        it.next();
        QString curr = parent + "[" + it.key() + "]";

        // Print current index
        output += curr + " = " + (!it.value()->val.isNull() ? it.value()->val.toString() : "null") + "\n";

        // Print next children from this dict
        output += buildString(it.value(), curr);
    }
    return output;
}

QString Dict::buildStringTree(const Dict *next, int level) const // NOLINT(misc-no-recursion)
{
    QString output = "";
    QMapIterator<QString, Dict *> it(next->map);

    QString tabs = "";
    for( int i = 0; i < level; i++ )
        tabs += "\t";

    while (it.hasNext())
    {
        it.next();

        // Print current index
        output += tabs + it.key() + " -> " + (!it.value()->val.isNull() ? it.value()->val.toString() : "null") + "\n";

        // Print next children from this dict
        output += buildStringTree(it.value(), level + 1);
    }
    return output;
}
