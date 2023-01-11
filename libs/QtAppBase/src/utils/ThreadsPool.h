#ifndef _SERVICES_THREADSPOOL_H_
#define _SERVICES_THREADSPOOL_H_

#include "extensions/QThreadPoolExtended.h"

class ThreadsPool
{
public:
    ThreadsPool(int max_threads_count = 5);
    ~ThreadsPool() = default;
    QThreadPoolExtended *ThreadsPoolPtr();
protected:

private:
    QThreadPoolExtended threadsPool;
};

#endif // _SERVICES_THREADSPOOL_H_