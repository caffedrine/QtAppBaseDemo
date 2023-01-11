#ifndef _APPSETTINGS_H_
#define _APPSETTINGS_H_

#include <QSettings>
#include <QString>

#define ApplSettings AppSettings->instance()

class AppSettings
{
public:
    Q_DISABLE_COPY(AppSettings)
    static AppSettings* inst();

protected:

private:
    static AppSettings *instance;
    QSettings *settings{};

    AppSettings();
};

#endif // _APPSETTINGS_H_
