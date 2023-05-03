#include <QCoreApplication>
#include "MainApp.h"

MainApp::MainApp(QObject *parent): QObject(parent)
{
    qInfo().nospace().noquote() << "[MainApp] Started main application";
}

void MainApp::exit(int retcode)
{
    QCoreApplication::exit(retcode);
    QCoreApplication::processEvents();
    std::exit(retcode);
}

MainApp::~MainApp()
{
    qInfo().nospace().noquote() << "[MainApp] Finished";
}
