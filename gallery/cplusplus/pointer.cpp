// pointer.cpp

#include <iostream>

void allocate(char*& buffer)
{
	buffer = new char[256];
}

void release(char*& buffer)
{
	delete[] buffer;
	buffer = 0;
}

// 在foo函数内部仅会影响buffer所指向地址的内容。
// 不会更改指向地址的关系。
void foo(char* buffer)
{
	buffer = new char[256];
}

// 在Delete函数内部通过引用更改指针所指向地址。
// 会更改指向地址的关系。
template<class T> void Delete(T& x)
{
	if (x) delete x;
	x = NULL;
}

template<class T> void DeleteArray(T& x)
{
	if (x) delete[] x;
	x = NULL;
}

int main(int argc, char* argv[])
{
	// case 1:
	int a1[3][4] = {{1,2,3,4}, {5,6,7,8}, {9,10,11,12}};
	int (*b1)[4];
	b1 = a1;
	std::cout << *(*b1+1) << std::endl;
	
	// case 2:
	int k[5] = {1,2,3,4,5};
	int *ptr = (int*)(&k+1); // unknown
	printf("%d,%d", *(k+1), *(ptr-1));
	std::cout << *(ptr-1)<< std::endl;


	// const int LENGTH = 512;
	// char* buffer = new char[LENGTH];
	char* buffer = NULL;
	allocate(buffer);
	release(buffer);

	return 0;
}
