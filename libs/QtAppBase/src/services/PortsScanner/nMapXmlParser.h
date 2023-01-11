#ifndef _SERVICES_NMAPXMLPARSER_H_
#define _SERVICES_NMAPXMLPARSER_H_

#include <QDomDocument>
#include <QList>

namespace Services { namespace PortsScanner
{

class nMapPortState
{
public:
    quint16 PortNumber;
    QString Protocol;
    QString State;
};

class nMapXmlParser
{
public:
    nMapXmlParser();
    nMapXmlParser(const QString &nMap_XML);
    ~nMapXmlParser();
    bool ParseXML(const QString &nMap_XML);
    bool IsValidXML();
    QString GetParsingErrorDesc();
    QString GetInputXML();
    QDomDocument const *GetInputDomDocument();

    bool GetNmapParam_ScanSucceed();
    QString GetNmapParam_TargetAddress();
    QString GetNmapParam_TargetRDNS();
    QString GetNmapParam_TargetState();
    QList<nMapPortState> GetNmapParam_OpenPorts();
    float GetNmapParam_LatencySeconds();

protected:

private:
    QString ParsingErrorDesc;
    QDomDocument xml;

};

}} // Namespaces

#endif // _SERVICES_NMAPXMLPARSER_H_
