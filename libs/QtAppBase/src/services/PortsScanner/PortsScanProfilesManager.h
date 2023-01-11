#ifndef _SERVICES_PORTSSCANPROFILESMANAGER_H_
#define _SERVICES_PORTSSCANPROFILESMANAGER_H_

#include <QObject>
#include "utils/Singleton.h"

#include "PortsScanBuiltInProfiles.h"

namespace Services { namespace PortsScanner
{

class PortsScanProfilesManager: protected QObject, public Singleton<PortsScanProfilesManager>
{
public:
    QList<PortsScanProfileType> GetAll();
    PortsScanProfileType GetByName(QString profile_name);

protected:

private:

};

}} // Namespaces

#endif // _SERVICES_PORTSSCANPROFILESMANAGER_H_
