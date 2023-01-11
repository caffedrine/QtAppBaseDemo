#include "AppSettings.h"

#include <QCoreApplication>
#include <QApplication>

AppSettings *AppSettings::instance = nullptr;

AppSettings::AppSettings()
{
    this->settings = new QSettings(QApplication::applicationDirPath() + "/settings.ini", QSettings::Format::IniFormat);
}

AppSettings *AppSettings::inst()
{
    if( instance == nullptr )
    {
        instance = new AppSettings();
    }
    return instance;
}
