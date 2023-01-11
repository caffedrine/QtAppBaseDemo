#ifndef _QTHREADPOOLEXTENDED_H_
#define _QTHREADPOOLEXTENDED_H_

#include <QThreadPool>

class QThreadPoolExtended: public QThreadPool
{
public:
    QThreadPoolExtended(QObject *parent = nullptr);
    ~QThreadPoolExtended()=default;

    int AvailableThreads();
    int ActiveThreads();

protected:

private:
};

#endif // _QTHREADPOOLEXTENDED_H_