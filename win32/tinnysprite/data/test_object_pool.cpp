/**
 * ≤‚ ‘∂‘œÛ≥ÿ
 */
#include "SSCommon.h"
#include "SSObjectPool.h"
#include <process.h>
using namespace SSDataDomain;

void thread_proc(void* parameter)
{
	SSObjectPool<int*>* pool = (SSObjectPool<int*>*) parameter;

	//list<int*> list;

	//while(true) {
	//	if ((rand()%2)) {
	//		list.push_back(pool->GetObject());
	//	} else {
	//		if (!list.empty()) {
	//			pool->SetObject(list.front());
	//			list.pop_front();
	//		}
	//	}
	//	
	//	pool->print();
	//}

	while(true) {
		int* o = pool->GetObject();
		::Sleep((rand()%3000));
		pool->SetObject(o);
		::Sleep((rand()%3000));
	}
}

int* create_func() {
	return new int();
}
void destroy_func(int* o) {
	delete o;
}

int main(int argc, char** argv)
{
	SSObjectPool<int*> pool;
	pool.SetMinSize(15);
	pool.SetMaxSize(1000);
	pool.SetCreateFunc(create_func);
	pool.SetDestroyFunc(destroy_func);
	
	HANDLE threads[100];
	int count = 0;
	for(count = 0; count < 20; count++) {
		threads[count] = (HANDLE)_beginthread(thread_proc, 0, &pool);
		::Sleep(rand()%5000);
	}

	::WaitForMultipleObjects(count, threads, TRUE, INFINITE);
}