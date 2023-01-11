#include "utils/ThreadsPool.h"

ThreadsPool::ThreadsPool(int max_threads_count)
{
    this->threadsPool.setMaxThreadCount(max_threads_count);
}

QThreadPoolExtended *ThreadsPool::ThreadsPoolPtr()
{
    return &this->threadsPool;
}
