#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <ASN1_Object.h>
#include <Compiler.h>
#include <utils/Logger.h>

#ifdef _WIN32
#include <Windows.h>
#endif

class X509Cert: protected ASN1_Object
{

};

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

    X509Cert cert;

    // Show debug info when app is executed from console
    #ifdef _WIN32
        // https://stackoverflow.com/questions/3360548/console-output-in-a-qt-gui-app
        if (AttachConsole(ATTACH_PARENT_PROCESS))
        {
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);
        }
    #endif


    if (qobject_cast<QApplication *>(app.data()))
    {
        base::setup_logger(false);
        base::logger->info("Started application in GUI mode (use flag --nogui to launch in console mode)");
        QMainWindow w;
        w.show();
        return app->exec();
    }
    else
    {
        base::setup_logger(true);
        base::logger->info("Start application in console mode");
        return app->exec();
    }
}
