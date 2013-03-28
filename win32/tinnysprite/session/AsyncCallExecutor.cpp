/*--------------------------------------------------------------------------------*\

     文件：  AsyncCallExecutor.cpp
     作者：  姚松文
     日期：  2010年10月20日
     说明：  异步调用的执行者。

            先通过AsyncCallExecutor::start启动executor，然后通过AsyncCallExecutor::
            submit(AsyncCall*)提交任务，异步调用逻辑由AsyncCall::call函数实现，executor
            将异步执行接收的任务，任务执行完后executor调用AsyncCall::dispose释放资源。
\*--------------------------------------------------------------------------------*/
#include "stdafx.h"
#include <queue>
#include <SSConcurrency.h>
#include "AsyncCallExecutor.h"

DEFINE_LOGGER(_T("stub.async"));

class AsyncCallExecutorImpl : public AsyncCallExecutor {
public:
    virtual void start();
    virtual void stop();
    virtual void submit(AsyncCall*);
public:
    AsyncCallExecutorImpl(int thread_count);
    ~AsyncCallExecutorImpl();
private:
    int thread_count;
    HANDLE* p_threads;
    SSDataDomain::SSLock queue_lock;
    HANDLE evtNotEmpty;
    std::queue<AsyncCall*> pending_call_queue;
    int do_stop;
    static void executor_thread(AsyncCallExecutorImpl* executor);
    void run();
    AsyncCall* pull();
};

AsyncCallExecutorImpl::AsyncCallExecutorImpl(int thread_count) {
    if (thread_count > MAXIMUM_WAIT_OBJECTS)
        thread_count = MAXIMUM_WAIT_OBJECTS;
    this->thread_count = thread_count;
    this->evtNotEmpty = NULL;
    this->p_threads = NULL;
}

AsyncCallExecutorImpl::~AsyncCallExecutorImpl() {
    if (this->evtNotEmpty != NULL) {
        ::CloseHandle(this->evtNotEmpty);
        this->evtNotEmpty = NULL;
    }
    if (this->p_threads != NULL) {
        delete[] this->p_threads;
        this->p_threads = NULL;
    }
}

void AsyncCallExecutorImpl::start() {
    do_stop = 0;
    this->evtNotEmpty = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    this->p_threads = new HANDLE[thread_count];
    for (int i = 0; i < thread_count; i++) {
        p_threads[i] = (HANDLE)_beginthread((void (_cdecl*)(void*))executor_thread, 0, this);
    }
}

void AsyncCallExecutorImpl::stop() {
    do_stop = 1;

    SSLOG_DEBUG(_T("stopping all async call thread."));
    ::SetEvent(this->evtNotEmpty);
    ::WaitForMultipleObjects(this->thread_count, this->p_threads, TRUE, INFINITE);
    SSLOG_DEBUG(_T("all async call thread is stopped."));

    if (this->evtNotEmpty != NULL) {
        ::CloseHandle(this->evtNotEmpty);
        this->evtNotEmpty = NULL;
    }
    if (this->p_threads != NULL) {
        delete[] this->p_threads;
        this->p_threads = NULL;
    }
}

void AsyncCallExecutorImpl::submit(AsyncCall* async_call) {
    queue_lock.lock();
    pending_call_queue.push(async_call);
    ::SetEvent(this->evtNotEmpty);
    queue_lock.unlock();
}

AsyncCall* AsyncCallExecutorImpl::pull() {
    AsyncCall* pending_call = NULL;
    queue_lock.lock();
    if (!pending_call_queue.empty()) {
        pending_call = pending_call_queue.front();
        pending_call_queue.pop();
    }
    queue_lock.unlock();
    return pending_call;
}

void AsyncCallExecutorImpl::run() {
    while (!do_stop) {
        AsyncCall* pending_call = pull();
        if (pending_call == NULL) {
            SSLOG_DEBUG(_T("thread [") << GetCurrentThreadId() << _T("] is sleep."));
            ::WaitForSingleObject(this->evtNotEmpty, INFINITE);
            SSLOG_DEBUG(_T("thread [") << GetCurrentThreadId() << _T("] wake up."));
            continue;
        }

        SSLOG_DEBUG(_T("starting call: ") << pending_call->get_index());

        try {
            pending_call->call();
        } catch(...) {
            SSLOG_ERROR(_T("exception happened in async call thread [") << GetCurrentThreadId() << _T("]."));
        }

        SSLOG_DEBUG(_T("finished call: ") << pending_call->get_index());

        pending_call->dispose();
    }
    // 通知其他阻塞的同伴退出
    ::SetEvent(this->evtNotEmpty);
    SSLOG_DEBUG(_T("thread [") << GetCurrentThreadId() << _T("] is exited."));
}

void AsyncCallExecutorImpl::executor_thread(AsyncCallExecutorImpl* executor) {
    try {
		executor->run();
	} catch(...) {
		SSLOG_ERROR(_T("thread [") << GetCurrentThreadId() << _T("] something wrong happened in async call thread."));
	}
	_endthread();
}

AsyncCallExecutorImpl impl(10);
AsyncCallExecutor* g_async_call_executor = &impl;