#ifndef _SERVICES_MMDBREADER_H_
#define _SERVICES_MMDBREADER_H_

#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QHostAddress>
#include <QtCore/QFile>
#include <QtCore/QJsonValue>

namespace Services { namespace GeoIP
{

struct DataFieldDescriptor;

class MmdbReader
{
    Q_DECLARE_TR_FUNCTIONS(CountryLookup)

public:
    static MmdbReader *load(const QString &filename, QString &error);
    static MmdbReader *load(const QByteArray &data, QString &error);

    ~MmdbReader();

    QString type() const;
    quint16 ipVersion() const;
    QDateTime buildEpoch() const;
    QVariant lookup(const QHostAddress &hostAddr) const;

private:
    explicit MmdbReader(quint32 size);

    bool parseMetadata(const QVariantHash &metadata, QString &error);
    bool loadDB(QString &error) const;
    QVariantHash readMetadata() const;

    QVariant readDataField(quint32 &offset) const;
    bool readDataFieldDescriptor(quint32 &offset, DataFieldDescriptor &out) const;
    void fromBigEndian(uchar *buf, quint32 len) const;
    QVariant readMapValue(quint32 &offset, quint32 count) const;
    QVariant readArrayValue(quint32 &offset, quint32 count) const;

    template<typename T>
    QVariant readPlainValue(quint32 &offset, quint8 len) const
    {
        T value = 0;
        const uchar *const data = m_data + offset;
        const quint32 availSize = m_size - offset;

        if ((len > 0) && (len <= sizeof(T) && (availSize >= len)))
        {
            // copy input data to last 'len' bytes of 'value'
            uchar *dst = reinterpret_cast<uchar *>(&value) + (sizeof(T) - len);
            memcpy(dst, data, len);
            fromBigEndian(reinterpret_cast<uchar *>(&value), sizeof(T));
            offset += len;
        }

        return QVariant::fromValue(value);
    }

    // Metadata
    quint16 m_ipVersion;
    quint16 m_recordSize;
    quint32 m_nodeCount;
    int m_nodeSize;
    int m_indexSize;
    int m_recordBytes;
    QDateTime m_buildEpoch;
    QString m_dbType;
    // Search data
    mutable QHash<quint32, QString> m_countries;
    quint32 m_size;
    uchar *m_data;
};

}} // Namespaces

#endif // _SERVICES_MMDBREADER_H_
