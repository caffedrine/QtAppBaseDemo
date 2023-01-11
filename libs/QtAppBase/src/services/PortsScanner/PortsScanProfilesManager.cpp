#include "PortsScanProfilesManager.h"

namespace Services { namespace PortsScanner
{

QList<PortsScanProfileType> PortsScanProfilesManager::GetAll()
{
    QList<PortsScanProfileType> output;

    output.append(PortsScanBuiltInProfiles::instance().GetAllProfiles());

    return output;
}

PortsScanProfileType PortsScanProfilesManager::GetByName(QString profile_name)
{
    PortsScanProfileType output;

    for( PortsScanProfileType target: this->GetAll())
    {
        if( target.ProfileName == profile_name )
            return target;
    }

    return output;
}

}} // Namespaces
