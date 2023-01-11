#include "X509Certificate.h"

#include <QSsl>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslCertificateExtension>

namespace Core { namespace Crypto { namespace x509 {
    QString GetCertHumanReadableFormat(const QByteArray &certBytes)
    {
        auto GetSubjInfoHumanReadable = [](const QSslCertificate &cert, bool subject) -> QString {
            QMap<int, QString> tagStr = {{QSslCertificate::SubjectInfo::Organization, "ORG"},
                                         {QSslCertificate::SubjectInfo::CommonName, "CN"},
                                         {QSslCertificate::SubjectInfo::LocalityName, "LN"},
                                         {QSslCertificate::SubjectInfo::OrganizationalUnitName, "OUN"},
                                         {QSslCertificate::SubjectInfo::CountryName, "C"},
                                         {QSslCertificate::SubjectInfo::StateOrProvinceName, "ST"},
                                         {QSslCertificate::SubjectInfo::DistinguishedNameQualifier, "DN"},
                                         {QSslCertificate::SubjectInfo::SerialNumber, "SN"},
                                         {QSslCertificate::SubjectInfo::EmailAddress, "EMAIL"}};
            QMap<int, QString>::iterator i;

            QString output = "";
            bool AlreadyStarted = false;
            for (i = tagStr.begin(); i != tagStr.end(); ++i)
            {
                QStringList currTag = subject ? cert.subjectInfo((QSslCertificate::SubjectInfo)i.key()) : cert.issuerInfo((QSslCertificate::SubjectInfo)i.key());
                if( !currTag.empty() )
                {
                    if( AlreadyStarted )
                        output += "; ";
                    output += i.value() + " = " + currTag.join(", ");
                    AlreadyStarted = true;
                }
            }
            return output;
        };

        // Parse certificate first using Qt
        QString output = "";
        const auto certs = QSslCertificate::fromData(certBytes, QSsl::Der);
        qInfo() << "Certificates found: " << certs.count();
        for (const QSslCertificate &cert : certs)
        {
            output += "Subject: " + GetSubjInfoHumanReadable(cert, true);
            if(cert.isSelfSigned())
                output += "\nSelf signed: true";
            output += "\nVersion: " + QString(cert.version());
            output += "\nSerial number: " + QString(cert.serialNumber());
            output += "\nIssuer: " + GetSubjInfoHumanReadable(cert, false);
            output += "\nValid not before: " + cert.effectiveDate().toString("yyyy-MM-dd hh:mm:ss") + " " + cert.effectiveDate().timeZoneAbbreviation();
            output += "\nValid not after: " + cert.expiryDate().toString("yyyy-MM-dd hh:mm:ss") + " " + cert.expiryDate().timeZoneAbbreviation();

//        output += "\nPublic key algorithm: " + QString::number((int)cert.publicKey().algorithm());
//        output += "\nPublic key len: " + QString::number(cert.publicKey().length());
//        output += "\nPublic key: " + cert.publicKey().toDer().toHex(':').toLower();
//        output += "\nExtensions: " + QString::number(cert.extensions().count());
//        certHumanReadable += "\nSignature algo       : ";
//        certHumanReadable += "\nSignature            : ";
            output += "\n";
        }
        if( certs.empty() )
            output += "No valid certificates found\n";

        return output;
    }

}}} //  namespaces