#ifndef _CFG_PORTSSCANNER_H_
#define _CFG_PORTSSCANNER_H_

#include "app/UserSettings.h"

namespace Services { namespace PortsScanner { namespace Settings
{
    static QStringList GetUserDataPaths()
    {
        return  QStringList( UserSettings->Get_UserDataPathAbs());
    }
}}} // Namespaces

#endif // _CFG_PORTSSCANNER_H_
