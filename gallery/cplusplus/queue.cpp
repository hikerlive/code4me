// queue.cpp

int main(int argc, char* argv[])
{
	return 0;
}

class Queue
{
public:
	Queue(int size);
	~Queue();

	char* Alloc(int size);
	void  Release(int size);

private:
	char* pfirst;
	int   nsize;
	char* pcurrent;
	int	  nidle;
};

Queue::Queue(int size)
{
	nsize = 10*1024*1024*sizeof(char);
	pfirst = (char*) malloc(nsize);
	if (pfirst == NULL) return;
	pcurrent = pfirst;
	nidle = nsize;
}

Queue::~Queue()
{
	if (pfirst != NULL)
	{
		free(pfirst);
		pfirst = NULL;
	}
}

char* Queue::Alloc(int size)
{
	if (nidle <= size)
	{
		char* p = pcurrent;
		pcurrent = pcurrent + size;
		nidle = nidle-size;
		return p;
	}
	else
	{
		cout << "no enough memory" << endl;
		return NULL;
	}
}

void Queue::Release(int size)
{
	nidle = nidle + size;
	pcurrent = pcurrent-size;
}
