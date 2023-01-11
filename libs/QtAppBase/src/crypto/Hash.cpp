#include <QCryptographicHash>
#include "Hash.h"

namespace Crypto { namespace Hash
{
    QByteArray MD5(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(input);
        return hash.result();
    }

    QByteArray SHA1(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Sha1);
        hash.addData(input);
        return hash.result();
    }

    QByteArray SHA2_224(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Sha224);
        hash.addData(input);
        return hash.result();
    }

    QByteArray SHA2_256(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Sha256);
        hash.addData(input);
        return hash.result();
    }

    QByteArray SHA2_384(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Sha384);
        hash.addData(input);
        return hash.result();
    }

    QByteArray SHA2_512(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Sha512);
        hash.addData(input);
        return hash.result();
    }

    QByteArray SHA2_512_224(const QByteArray &input)
    {
//        unsigned char digest[SHA512_224::DIGEST_SIZE];
//        memset(digest,0,SHA512_224::DIGEST_SIZE);
//
//        SHA512_224 ctx = SHA512_224();
//        ctx.init();
//        ctx.update((unsigned char *)input.constData(), input.size());
//        ctx.final(digest);
//
//        return QByteArray((char *)digest, SHA512_224::DIGEST_SIZE);
        return QByteArray();
    }

    QByteArray SHA2_512_256(const QByteArray &input)
    {
        return QByteArray();
    }

    QByteArray SHA3_224(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Sha3_224);
        hash.addData(input);
        return hash.result();
    }

    QByteArray SHA3_256(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Sha3_256);
        hash.addData(input);
        return hash.result();
    }

    QByteArray SHA3_384(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Sha3_384);
        hash.addData(input);
        return hash.result();
    }

    QByteArray SHA3_512(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Sha3_512);
        hash.addData(input);
        return hash.result();
    }

    QByteArray KECCAK_224(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Keccak_224);
        hash.addData(input);
        return hash.result();
    }

    QByteArray KECCAK_256(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Keccak_256);
        hash.addData(input);
        return hash.result();
    }

    QByteArray KECCAK_384(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Keccak_384);
        hash.addData(input);
        return hash.result();
    }

    QByteArray KECCAK_512(const QByteArray &input)
    {
        QCryptographicHash hash(QCryptographicHash::Keccak_512);
        hash.addData(input);
        return hash.result();
    }

    QByteArray BLAKE2B_160(const QByteArray &input)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QCryptographicHash hash(QCryptographicHash::Blake2b_160);
        hash.addData(input);
        return hash.result();
#else
        Q_UNUSED(input);
        return QByteArray();
#endif
    }

    QByteArray BLAKE2B_256(const QByteArray &input)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

        QCryptographicHash hash(QCryptographicHash::Blake2b_256);
        hash.addData(input);
        return hash.result();
#else
        Q_UNUSED(input);
        return QByteArray();
#endif
    }

    QByteArray BLAKE2B_384(const QByteArray &input)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QCryptographicHash hash(QCryptographicHash::Blake2b_384);
        hash.addData(input);
        return hash.result();
#else
        Q_UNUSED(input);
        return QByteArray();
#endif
    }

    QByteArray BLAKE2B_512(const QByteArray &input)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QCryptographicHash hash(QCryptographicHash::Blake2b_512);
        hash.addData(input);
        return hash.result();
#else
        Q_UNUSED(input);
        return QByteArray();
#endif
    }

    QByteArray BLAKE2S_128(const QByteArray &input)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QCryptographicHash hash(QCryptographicHash::Blake2s_128);
        hash.addData(input);
        return hash.result();
#else
        Q_UNUSED(input);
        return QByteArray();
#endif
    }

    QByteArray BLAKE2S_160(const QByteArray &input)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QCryptographicHash hash(QCryptographicHash::Blake2s_160);
        hash.addData(input);
        return hash.result();
#else
        Q_UNUSED(input);
        return QByteArray();
#endif
    }

    QByteArray BLAKE2S_224(const QByteArray &input)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QCryptographicHash hash(QCryptographicHash::Blake2s_224);
        hash.addData(input);
        return hash.result();
#else
        Q_UNUSED(input);
        return QByteArray();
#endif
    }

    QByteArray BLAKE2S_256(const QByteArray &input)
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QCryptographicHash hash(QCryptographicHash::Blake2s_256);
        hash.addData(input);
        return hash.result();
#else
        Q_UNUSED(input);
        return QByteArray();
#endif
    }
}} // Namespaces
