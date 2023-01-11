#include "MmdbReader.h"

namespace Services { namespace GeoIP
{

namespace
{
    const qint32 MAX_FILE_SIZE = 67108864; // 64MB
    const quint32 MAX_METADATA_SIZE = 131072; // 128KB
    const char METADATA_BEGIN_MARK[] = "\xab\xcd\xefMaxMind.com";
    const char DATA_SECTION_SEPARATOR[16] = {0};

    enum class DataType
    {
        Unknown = 0,
        Pointer = 1,
        String = 2,
        Double = 3,
        Bytes = 4,
        Integer16 = 5,
        Integer32 = 6,
        Map = 7,
        SignedInteger32 = 8,
        Integer64 = 9,
        Integer128 = 10,
        Array = 11,
        DataCacheContainer = 12,
        EndMarker = 13,
        Boolean = 14,
        Float = 15
    };
}

struct DataFieldDescriptor
{
    DataType fieldType{DataType::Unknown};
    union
    {
        quint32 fieldSize = 0;
        quint32 offset; // Pointer
    };
};

MmdbReader::MmdbReader(const quint32 size) : m_ipVersion(0), m_recordSize(0), m_nodeCount(0), m_nodeSize(0), m_indexSize(0), m_recordBytes(0), m_size(size), m_data(new uchar[size])
{
}

MmdbReader *MmdbReader::load(const QString &filename, QString &error)
{
    MmdbReader *db = nullptr;
    QFile file(filename);
    if (file.size() > MAX_FILE_SIZE)
    {
        error = tr("Unsupported database file size.");
        return nullptr;
    }

    if (!file.open(QFile::ReadOnly))
    {
        error = file.errorString();
        return nullptr;
    }


    db = new MmdbReader(file.size());

    if (file.read(reinterpret_cast<char *>(db->m_data), db->m_size) != db->m_size)
    {
        error = file.errorString();
        delete db;
        return nullptr;
    }


    if (!db->parseMetadata(db->readMetadata(), error) || !db->loadDB(error))
    {
        delete db;
        return nullptr;
    }

    return db;
}

MmdbReader *MmdbReader::load(const QByteArray &data, QString &error)
{
    MmdbReader *db = nullptr;
    if (data.size() > MAX_FILE_SIZE)
    {
        error = tr("Unsupported database file size.");
        return nullptr;
    }

    db = new MmdbReader(data.size());

    memcpy(reinterpret_cast<char *>(db->m_data), data.constData(), db->m_size);

    if (!db->parseMetadata(db->readMetadata(), error) || !db->loadDB(error))
    {
        delete db;
        return nullptr;
    }

    return db;
}

MmdbReader::~MmdbReader()
{
    delete[] m_data;
}

QString MmdbReader::type() const
{
    return m_dbType;
}

quint16 MmdbReader::ipVersion() const
{
    return m_ipVersion;
}

QDateTime MmdbReader::buildEpoch() const
{
    return m_buildEpoch;
}

QVariant MmdbReader::lookup(const QHostAddress &hostAddr) const
{
    Q_IPV6ADDR addr = hostAddr.toIPv6Address();

    const uchar *ptr = m_data;

    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            const bool right = static_cast<bool>((addr[i] >> (7 - j)) & 1);
            // Interpret the left/right record as number
            if (right)
                ptr += m_recordBytes;

            quint32 id = 0;
            auto *idPtr = reinterpret_cast<uchar *>(&id);
            memcpy(&idPtr[4 - m_recordBytes], ptr, m_recordBytes);
            fromBigEndian(idPtr, 4);

            if (id == m_nodeCount)
            {
                return {};
            }

            if (id > m_nodeCount)
            {
                const quint32 offset = id - m_nodeCount - sizeof(DATA_SECTION_SEPARATOR);
                quint32 tmp = offset + m_indexSize + sizeof(DATA_SECTION_SEPARATOR);
                const QVariant val = readDataField(tmp);
                if (val.userType() == QMetaType::QVariantHash)
                {
                    return val;
                }
            }

            ptr = m_data + (id * m_nodeSize);
        }
    }

    return {};
}

#define CHECK_METADATA_REQ(key, type) \
if (!metadata.contains(#key)) \
{ \
    error = errMsgNotFound.arg(#key); \
    return false; \
} \
if (metadata.value(#key).userType() != QMetaType::type) \
{ \
    error = errMsgInvalid.arg(#key);  \
    return false; \
}

#define CHECK_METADATA_OPT(key, type) \
if (metadata.contains(#key)) \
{ \
    if (metadata.value(#key).userType() != QMetaType::type) \
    { \
        error = errMsgInvalid.arg(#key);  \
        return false; \
    } \
}

bool MmdbReader::parseMetadata(const QVariantHash &metadata, QString &error)
{
    const QString errMsgNotFound = tr("Metadata error: '%1' entry not found.");
    const QString errMsgInvalid = tr("Metadata error: '%1' entry has invalid type.");

    //qDebug() << "Parsing MaxMindDB metadata...";

    CHECK_METADATA_REQ(binary_format_major_version, UShort);
    CHECK_METADATA_REQ(binary_format_minor_version, UShort);
    const uint versionMajor = metadata.value("binary_format_major_version").toUInt();
    const uint versionMinor = metadata.value("binary_format_minor_version").toUInt();
    if (versionMajor != 2)
    {
        error = tr("Unsupported database version: %1.%2").arg(versionMajor).arg(versionMinor);
        return false;
    }

    CHECK_METADATA_REQ(ip_version, UShort);
    m_ipVersion = metadata.value("ip_version").value<quint16>();
    if (m_ipVersion != 6)
    {
        error = tr("Unsupported IP version: %1").arg(m_ipVersion);
        return false;
    }

    CHECK_METADATA_REQ(record_size, UShort);
    m_recordSize = metadata.value("record_size").value<quint16>();
    if (m_recordSize != 24)
    {
        error = tr("Unsupported record size: %1").arg(m_recordSize);
        return false;
    }
    m_nodeSize = m_recordSize / 4;
    m_recordBytes = m_nodeSize / 2;

    CHECK_METADATA_REQ(node_count, UInt);
    m_nodeCount = metadata.value("node_count").value<quint32>();
    m_indexSize = m_nodeCount * m_nodeSize;

    CHECK_METADATA_REQ(database_type, QString);
    m_dbType = metadata.value("database_type").toString();

    CHECK_METADATA_REQ(build_epoch, ULongLong);
    m_buildEpoch = QDateTime::fromSecsSinceEpoch(metadata.value("build_epoch").toULongLong());

    CHECK_METADATA_OPT(languages, QVariantList);
    CHECK_METADATA_OPT(description, QVariantHash);

    return true;
}

bool MmdbReader::loadDB(QString &error) const
{
    //qDebug() << "Parsing IP geolocation database index tree...";

    const int nodeSize = m_recordSize / 4; // in bytes
    const int indexSize = m_nodeCount * nodeSize;
    if ((m_size < (indexSize + sizeof(DATA_SECTION_SEPARATOR)))
        || (memcmp(m_data + indexSize, DATA_SECTION_SEPARATOR, sizeof(DATA_SECTION_SEPARATOR)) != 0))
    {
        error = tr("Database corrupted: no data section found.");
        return false;
    }

    return true;
}

QVariantHash MmdbReader::readMetadata() const
{
    const char *ptr = reinterpret_cast<const char *>(m_data);
    quint32 size = m_size;
    if (m_size > MAX_METADATA_SIZE)
    {
        ptr += m_size - MAX_METADATA_SIZE;
        size = MAX_METADATA_SIZE;
    }

    const QByteArray data = QByteArray::fromRawData(ptr, size);
    int index = data.lastIndexOf(METADATA_BEGIN_MARK);
    if (index >= 0)
    {
        if (m_size > MAX_METADATA_SIZE)
            index += (m_size - MAX_METADATA_SIZE); // from begin of all data
        auto offset = static_cast<quint32>(index + strlen(METADATA_BEGIN_MARK));
        const QVariant metadata = readDataField(offset);
        if (metadata.userType() == QMetaType::QVariantHash)
            return metadata.toHash();
    }

    return {};
}

QVariant MmdbReader::readDataField(quint32 &offset) const
{
    DataFieldDescriptor descr;
    if (!readDataFieldDescriptor(offset, descr))
        return {};

    quint32 locOffset = offset;
    bool usePointer = false;
    if (descr.fieldType == DataType::Pointer)
    {
        usePointer = true;
        // convert offset from data section to global
        locOffset = descr.offset + (m_nodeCount * m_recordSize / 4) + sizeof(DATA_SECTION_SEPARATOR);
        if (!readDataFieldDescriptor(locOffset, descr))
            return {};
    }

    QVariant fieldValue;
    switch (descr.fieldType)
    {
        case DataType::Pointer:
            qDebug() << "* Illegal Pointer using";
            break;
        case DataType::String:
            fieldValue = QString::fromUtf8(reinterpret_cast<const char *>(m_data + locOffset), descr.fieldSize);
            locOffset += descr.fieldSize;
            break;
        case DataType::Double:
            if (descr.fieldSize == 8)
                fieldValue = readPlainValue<double>(locOffset, descr.fieldSize);
            else
                qDebug() << "* Invalid field size for type: Double";
            break;
        case DataType::Bytes:
            fieldValue = QByteArray(reinterpret_cast<const char *>(m_data + locOffset), descr.fieldSize);
            locOffset += descr.fieldSize;
            break;
        case DataType::Integer16:
            fieldValue = readPlainValue<quint16>(locOffset, descr.fieldSize);
            break;
        case DataType::Integer32:
            fieldValue = readPlainValue<quint32>(locOffset, descr.fieldSize);
            break;
        case DataType::Map:
            fieldValue = readMapValue(locOffset, descr.fieldSize);
            break;
        case DataType::SignedInteger32:
            fieldValue = readPlainValue<qint32>(locOffset, descr.fieldSize);
            break;
        case DataType::Integer64:
            fieldValue = readPlainValue<quint64>(locOffset, descr.fieldSize);
            break;
        case DataType::Integer128:
            qDebug() << "* Unsupported data type: Integer128";
            break;
        case DataType::Array:
            fieldValue = readArrayValue(locOffset, descr.fieldSize);
            break;
        case DataType::DataCacheContainer:
            qDebug() << "* Unsupported data type: DataCacheContainer";
            break;
        case DataType::EndMarker:
            qDebug() << "* Unsupported data type: EndMarker";
            break;
        case DataType::Boolean:
            fieldValue = QVariant::fromValue(static_cast<bool>(descr.fieldSize));
            break;
        case DataType::Float:
            if (descr.fieldSize == 4)
                fieldValue = readPlainValue<float>(locOffset, descr.fieldSize);
            else
                qDebug() << "* Invalid field size for type: Float";
            break;
        default:
            qDebug() << "* Unsupported data type: Unknown";
    }

    if (!usePointer)
        offset = locOffset;
    return fieldValue;
}

bool MmdbReader::readDataFieldDescriptor(quint32 &offset, DataFieldDescriptor &out) const
{
    const uchar *dataPtr = m_data + offset;
    const int availSize = m_size - offset;
    if (availSize < 1) return false;

    out.fieldType = static_cast<DataType>((dataPtr[0] & 0xE0) >> 5);
    if (out.fieldType == DataType::Pointer)
    {
        const int size = ((dataPtr[0] & 0x18) >> 3);
        if (availSize < (size + 2)) return false;

        if (size == 0)
            out.offset = ((dataPtr[0] & 0x07) << 8) + dataPtr[1];
        else if (size == 1)
            out.offset = ((dataPtr[0] & 0x07) << 16) + (dataPtr[1] << 8) + dataPtr[2] + 2048;
        else if (size == 2)
            out.offset = ((dataPtr[0] & 0x07) << 24) + (dataPtr[1] << 16) + (dataPtr[2] << 8) + dataPtr[3] + 526336;
        else if (size == 3)
            out.offset = (dataPtr[1] << 24) + (dataPtr[2] << 16) + (dataPtr[3] << 8) + dataPtr[4];

        offset += size + 2;
        return true;
    }

    out.fieldSize = dataPtr[0] & 0x1F;
    if (out.fieldSize <= 28)
    {
        if (out.fieldType == DataType::Unknown)
        {
            out.fieldType = static_cast<DataType>(dataPtr[1] + 7);
            if ((out.fieldType <= DataType::Map) || (out.fieldType > DataType::Float) || (availSize < 3))
                return false;
            offset += 2;
        }
        else
        {
            offset += 1;
        }
    }
    else if (out.fieldSize == 29)
    {
        if (availSize < 2) return false;
        out.fieldSize = dataPtr[1] + 29;
        offset += 2;
    }
    else if (out.fieldSize == 30)
    {
        if (availSize < 3) return false;
        out.fieldSize = (dataPtr[1] << 8) + dataPtr[2] + 285;
        offset += 3;
    }
    else if (out.fieldSize == 31)
    {
        if (availSize < 4) return false;
        out.fieldSize = (dataPtr[1] << 16) + (dataPtr[2] << 8) + dataPtr[3] + 65821;
        offset += 4;
    }

    return true;
}

void MmdbReader::fromBigEndian(uchar *buf, const quint32 len) const
{
#if (Q_BYTE_ORDER == Q_LITTLE_ENDIAN)
    std::reverse(buf, buf + len);
#else
    Q_UNUSED(buf);
    Q_UNUSED(len);
#endif
}

QVariant MmdbReader::readMapValue(quint32 &offset, const quint32 count) const
{
    QVariantHash map;

    for (quint32 i = 0; i < count; ++i)
    {
        QVariant field = readDataField(offset);
        if (field.userType() != QMetaType::QString)
            return {};

        const QString key = field.toString();
        field = readDataField(offset);
        if (field.userType() == QVariant::Invalid)
            return {};

        map[key] = field;
    }

    return map;
}

QVariant MmdbReader::readArrayValue(quint32 &offset, const quint32 count) const
{
    QVariantList array;

    for (quint32 i = 0; i < count; ++i)
    {
        const QVariant field = readDataField(offset);
        if (field.userType() == QVariant::Invalid)
            return {};

        array.append(field);
    }

    return array;
}

}} // Namespaces
