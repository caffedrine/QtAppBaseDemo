#include "MainConsole.h"
#include <QEventLoop>
#include <QThread>
#include <base/utils/utils.h>

#include "src/ApplSettings.h"

MainConsole::MainConsole()
{
    this->init();
}

MainConsole::~MainConsole()
{
    delete this->app;
}

void MainConsole::exit(int retcode)
{
    QCoreApplication::exit(retcode);
    QCoreApplication::processEvents();
    std::exit(retcode);
}

void MainConsole::init()
{
    QCommandLineParser parser;
    parser.setApplicationDescription(PROJECT_DESC);
    parser.addVersionOption();

    // Option help
    QCommandLineOption helpOption({"?", "h", "help"}, QCoreApplication::translate("main", "Displays help on commandline options"));
    parser.addOption(helpOption);

    // Option nogui
    QCommandLineOption noguiOption("nogui", QCoreApplication::translate("main", "Load application in console mode"));
    parser.addOption(noguiOption);

    // Debugging messages level
    QCommandLineOption dbgLevelV({"v", "verbose"}, QCoreApplication::translate("main", "Enable verbose mode to print messages"));
    parser.addOption(dbgLevelV);

    QCommandLineOption dbgLevelVv({"vv", "vverbose"}, QCoreApplication::translate("main", "Enable super verbose mode to print all debugging messages"));
    parser.addOption(dbgLevelVv);

    // Process the actual command line arguments given by the user
    parser.process(*QCoreApplication::instance());

    // Check logging options
    if( parser.isSet(dbgLevelV) )
    {
        ApplSettings::instance().LoggingVerbose = true;
        ApplSettings::instance().LoggingSuperVerbose = false;
    }

    if( parser.isSet(dbgLevelVv) )
    {
        ApplSettings::instance().LoggingVerbose = true;
        ApplSettings::instance().LoggingSuperVerbose = true;
    }

    // Create a new app instance
    this->app = new MainApp(this);
}

