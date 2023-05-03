#include <QApplication>
#include <csignal>
#include <base/utils/Logger.h>

#include "ui/MainWindow.h"
#include "ui/MainConsole.h"
#include "src/gendata/Config.h"

#ifdef _WIN32
#include <Windows.h>
#endif

QCoreApplication* createApplication(int &argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        if (!qstrcmp(argv[i], "--nogui"))
            return new QCoreApplication(argc, argv);
    }
    return new QApplication(argc, argv);
}

int main(int argc, char *argv[])
{
    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));
    QCoreApplication::setApplicationName(PROJECT_NAME);
    QCoreApplication::setApplicationVersion(PROJECT_VER);

    // Show debug info when app is executed from console
#ifdef _WIN32
    // https://stackoverflow.com/questions/3360548/console-output-in-a-qt-gui-app
    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        //freopen("CONIN$", "r", stdin);
    }
#endif

    // Handle killing signals
    signal(SIGTERM, [](int sig) { qApp->quit(); });
    signal(SIGABRT, [](int sig) { qApp->quit(); });
    signal(SIGINT, [](int sig) { qApp->quit(); });
    //signal(SIGKILL, [](int sig){ qApp->quit(); });

    if (qobject_cast<QApplication *>(app.data()))
    {
        base::setup_logger(false);
        base::logger->info("Start application in GUI mode (use flag --nogui to launch in console mode)");
        MainWindow w;
        w.show();
        return app->exec();
    }
    else
    {
        base::setup_logger(true);
        base::logger->info("Start application in console mode");
        MainConsole c;
        return app->exec();
    }
}
