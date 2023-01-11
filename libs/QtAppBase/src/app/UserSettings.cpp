#include "UserSettings.h"

#include <QCoreApplication>
#include <QApplication>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>

User_Settings *User_Settings::instance = nullptr;

User_Settings::User_Settings()
{
    QString UserSettingsBasePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +  "/." + QFileInfo(QCoreApplication::applicationFilePath()).completeBaseName();

    this->settings = new QSettings( UserSettingsBasePath + "/user_settings.ini", QSettings::Format::IniFormat);

    // Create data path if it does not exists
    if( !QDir(UserSettingsBasePath).exists() )
    {
        QDir().mkdir(UserSettingsBasePath);
    }

    // Create data path if it does not exists
    if( !QDir(UserSettingsBasePath + "/data").exists() )
    {
        QDir().mkdir(UserSettingsBasePath + "/data");
    }
}

User_Settings *User_Settings::inst()
{
    if( instance == nullptr )
    {
        instance = new User_Settings();
    }
    return instance;
}

QString User_Settings::Get_UserBasePathAbs()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +  "/." + QFileInfo(QCoreApplication::applicationFilePath()).completeBaseName();
}

QString User_Settings::Get_UserDataPathAbs()
{
    return Get_UserBasePathAbs() + "/data";
}

QString User_Settings::Get_OSINT_ShodanApiKey()
{
    return QByteArray::fromBase64(this->settings->value("osint/shodan_api_key", "").toString().toUtf8());
}

void User_Settings::Set_OSINT_ShodanApiKey(QString newKey)
{
    this->settings->setValue("osint/shodan_api_key", newKey.toLocal8Bit().toBase64());
}

QByteArray User_Settings::decryptData(QByteArray &encrypted)
{
    // TODO: Implement

    return encrypted.toBase64();
}

QByteArray User_Settings::encryptData(QByteArray &decrypted)
{
    // TODO: Implement
    return QByteArray::fromBase64(decrypted);
}
