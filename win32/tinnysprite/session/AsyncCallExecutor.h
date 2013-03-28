/*--------------------------------------------------------------------------------*\

     文件：  AsyncCallExecutor.h
     作者：  姚松文
     日期：  2010年10月20日
     说明：  异步调用的执行者。

            先通过AsyncCallExecutor::start启动executor，然后通过AsyncCallExecutor::
            submit(AsyncCall*)提交任务，异步调用逻辑由AsyncCall::call函数实现，executor
            将异步执行接收的任务，任务执行完后executor调用AsyncCall::dispose释放资源。

\*--------------------------------------------------------------------------------*/

#ifndef ASYNC_CALL_EXECUTOR_H
#define ASYNC_CALL_EXECUTOR_H

#include <memory.h>

class AsyncCall {
    public: virtual unsigned int get_index() = 0;
    public: virtual void call() = 0;
    public: virtual void dispose() = 0;
};

class AsyncCallExecutor {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void submit(AsyncCall*) = 0;
};

extern AsyncCallExecutor* g_async_call_executor;

template<typename T>
T* reallocate(T* p, size_t size) {
    T* t = (T*)new unsigned char[size];
    memcpy((void*)t, (void*)p, size);
    return t;
}
template<typename T>
void release(T* p) {
    delete p;
}

#define BEGIN_ASYNC_CALL1(t1, n1) \
    class ClassX:public AsyncCall{public: \
    t1 n1; \
    ClassX(t1 n1){ \
        this->n1 = n1; \
        this->ctor(); \
    } \
    ~ClassX() { \
        this->dtor(); \
    } \
    unsigned int get_index() { \
        return (unsigned int)this->index; \
    } \
    void dispose() { \
        delete this; \
    }

#define END_ASYNC_CALL1(n1) \
    }; \
    g_async_call_executor->submit(new ClassX(n1));

#define BEGIN_ASYNC_CALL2(t1, n1, t2, n2) \
    class ClassX:public AsyncCall{public: \
    t1 n1; \
    t2 n2; \
    ClassX(t1 n1, t2 n2){ \
        this->n1 = n1; \
        this->n2 = n2; \
        this->ctor(); \
    } \
    ~ClassX() { \
        this->dtor(); \
    } \
    unsigned int get_index() { \
        return (unsigned int)this->index; \
    } \
    void dispose() { \
        delete this; \
    }

#define END_ASYNC_CALL2(n1, n2) \
    }; \
    g_async_call_executor->submit(new ClassX(n1, n2));

#define BEGIN_ASYNC_CALL3(t1, n1, t2, n2, t3, n3) \
    class ClassX:public AsyncCall{public: \
    t1 n1; \
    t2 n2; \
    t3 n3; \
    ClassX(t1 n1, t2 n2, t3 n3){ \
        this->n1 = n1; \
        this->n2 = n2; \
        this->n3 = n3; \
        this->ctor(); \
    } \
    ~ClassX() { \
        this->dtor(); \
    } \
    unsigned int get_index() { \
        return (unsigned int)this->index; \
    } \
    void dispose() { \
        delete this; \
    }

#define END_ASYNC_CALL3(n1, n2, n3) \
    }; \
    g_async_call_executor->submit(new ClassX(n1, n2, n3));

#define BEGIN_ASYNC_CALL4(t1, n1, t2, n2, t3, n3, t4, n4) \
    class ClassX:public AsyncCall{public: \
    t1 n1; \
    t2 n2; \
    t3 n3; \
    t4 n4; \
    ClassX(t1 n1, t2 n2, t3 n3, t4 n4){ \
        this->n1 = n1; \
        this->n2 = n2; \
        this->n3 = n3; \
        this->n4 = n4; \
        this->ctor(); \
    } \
    ~ClassX() { \
        this->dtor(); \
    } \
    unsigned int get_index() { \
        return (unsigned int)this->index; \
    } \
    void dispose() { \
        delete this; \
    }

#define END_ASYNC_CALL4(n1, n2, n3, n4) \
    }; \
    g_async_call_executor->submit(new ClassX(n1, n2, n3, n4));

#define BEGIN_ASYNC_CALL5(t1, n1, t2, n2, t3, n3, t4, n4, t5, n5) \
    class ClassX:public AsyncCall{public: \
    t1 n1; \
    t2 n2; \
    t3 n3; \
    t4 n4; \
    t5 n5; \
    ClassX(t1 n1, t2 n2, t3 n3, t4 n4, t5 n5){ \
        this->n1 = n1; \
        this->n2 = n2; \
        this->n3 = n3; \
        this->n4 = n4; \
        this->n5 = n5; \
        this->ctor(); \
    } \
    ~ClassX() { \
        this->dtor(); \
    } \
    unsigned int get_index() { \
        return (unsigned int)this->index; \
    } \
    void dispose() { \
        delete this; \
    }

#define END_ASYNC_CALL5(n1, n2, n3, n4, n5) \
    }; \
    g_async_call_executor->submit(new ClassX(n1, n2, n3, n4, n5));

#define BEGIN_ASYNC_CALL6(t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6) \
    class ClassX:public AsyncCall{public: \
    t1 n1; \
    t2 n2; \
    t3 n3; \
    t4 n4; \
    t5 n5; \
    t6 n6; \
    ClassX(t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6){ \
        this->n1 = n1; \
        this->n2 = n2; \
        this->n3 = n3; \
        this->n4 = n4; \
        this->n5 = n5; \
        this->n6 = n6; \
        this->ctor(); \
    } \
    ~ClassX() { \
        this->dtor(); \
    } \
    unsigned int get_index() { \
        return (unsigned int)this->index; \
    } \
    void dispose() { \
        delete this; \
    }

#define END_ASYNC_CALL6(n1, n2, n3, n4, n5, n6) \
    }; \
    g_async_call_executor->submit(new ClassX(n1, n2, n3, n4, n5, n6));

#define BEGIN_ASYNC_CALL7(t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7) \
    class ClassX:public AsyncCall{public: \
    t1 n1; \
    t2 n2; \
    t3 n3; \
    t4 n4; \
    t5 n5; \
    t6 n6; \
    t7 n7; \
    ClassX(t1 n1, t2 n2, t3 n3, t4 n4, t5 n5, t6 n6, t7 n7){ \
        this->n1 = n1; \
        this->n2 = n2; \
        this->n3 = n3; \
        this->n4 = n4; \
        this->n5 = n5; \
        this->n6 = n6; \
        this->n7 = n7; \
        this->ctor(); \
    } \
    ~ClassX() { \
        this->dtor(); \
    } \
    unsigned int get_index() { \
        return (unsigned int)this->index; \
    } \
    void dispose() { \
        delete this; \
    }

#define END_ASYNC_CALL7(n1, n2, n3, n4, n5, n6, n7) \
    }; \
    g_async_call_executor->submit(new ClassX(n1, n2, n3, n4, n5, n6, n7));

#endif //ASYNC_CALL_EXECUTOR_H