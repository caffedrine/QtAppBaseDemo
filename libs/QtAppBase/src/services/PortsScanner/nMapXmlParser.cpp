#include "nMapXmlParser.h"

namespace Services { namespace PortsScanner
{

nMapXmlParser::nMapXmlParser()
{
    this->ParsingErrorDesc = "No input";
}

nMapXmlParser::nMapXmlParser(const QString &nMap_XML)
{
    this->ParseXML(nMap_XML);
}

nMapXmlParser::~nMapXmlParser()
{

}

bool nMapXmlParser::IsValidXML()
{
    return this->ParsingErrorDesc.isEmpty();
}

QString nMapXmlParser::GetParsingErrorDesc()
{
    return this->ParsingErrorDesc;
}

QString nMapXmlParser::GetInputXML()
{
    return this->xml.toString(4);
}

QString nMapXmlParser::GetNmapParam_TargetAddress()
{
    return this->xml.documentElement().elementsByTagName("address").item(0).toElement().attribute("addr");
}

QString nMapXmlParser::GetNmapParam_TargetRDNS()
{
    return this->xml.documentElement().elementsByTagName("hostname").item(0).toElement().attribute("name");
}

QString nMapXmlParser::GetNmapParam_TargetState()
{
    QString state = this->xml.documentElement().elementsByTagName("status").item(0).toElement().attribute("state").toUpper();
    return (state.isEmpty() ? "DOWN" : state);
}

bool nMapXmlParser::ParseXML(const QString &nMap_XML)
{
    this->ParsingErrorDesc = "";
    this->xml.setContent(nMap_XML, &this->ParsingErrorDesc);
    return this->IsValidXML();
}

QDomDocument const *nMapXmlParser::GetInputDomDocument()
{
    return &this->xml;
}

QList<nMapPortState> nMapXmlParser::GetNmapParam_OpenPorts()
{
    QList<nMapPortState> output;

    // Update portslist scanned
    QDomNodeList ports = this->xml.documentElement().elementsByTagName("port");
    for( int j = 0; j < ports.count(); j++ )
    {
        nMapPortState port;
        port.State = ports.item(j).toElement().elementsByTagName("state").item(0).toElement().attribute("state");
        if( port.State.toLower() == "open" )
        {
            port.PortNumber = ports.item(j).toElement().attribute("portid").toInt();
            port.Protocol = ports.item(j).toElement().attribute("protocol");
            output.append(port);
        }
    }

    return output;
}

bool nMapXmlParser::GetNmapParam_ScanSucceed()
{
    return this->xml.documentElement().elementsByTagName("finished").item(0).toElement().attribute("exit").toLower() == "success";
}

float nMapXmlParser::GetNmapParam_LatencySeconds()
{
    float microseconds = this->xml.documentElement().elementsByTagName("times").item(0).toElement().attribute("srtt").toFloat();
    return  microseconds / 1000 / 1000;
}

}} // Namespaces
