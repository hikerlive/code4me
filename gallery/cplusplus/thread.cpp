#include<Windows.h>
#include<process.h>

class ThreadManager
{
public:
	ThreadManager(){}
	~ThreadManager(){}

	static unsigned int __stdcall WorkerThread(void* argument);
};

unsigned int __stdcall ThreadManager::WorkerThread(void* argument)
{
	return (0);
}

int main(int argc, char** argv)
{
	unsigned int dwAddress = 0;
	HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, ThreadManager::WorkerThread, NULL, 0, &dwAddress);
	return 0;
};