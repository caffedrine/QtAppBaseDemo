#ifndef _MAINCONSOLE_H_
#define _MAINCONSOLE_H_

#include <QObject>
#include <QDebug>
#include <QCoreApplication>
#include <QCommandLineParser>
#include "src/gendata/Config.h"
#include "src/MainApp.h"

class MainConsole: public QObject
{
    Q_OBJECT
    MainApp *app;

public:
    MainConsole();
    ~MainConsole() override;

private:
    void init();
    void exit(int retcode);
};


#endif //_MAINCONSOLE_H_
