#include "QThreadPoolExtended.h"

QThreadPoolExtended::QThreadPoolExtended(QObject *parent): QThreadPool(parent)
{

}

int QThreadPoolExtended::AvailableThreads()
{
    return (this->maxThreadCount() - this->activeThreadCount());
}

int QThreadPoolExtended::ActiveThreads()
{
    return this->activeThreadCount();
}


