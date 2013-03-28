#include "stlutils.h"

class CTest
{
public:
	CTest(){};
	~CTest(){};

	int id;
};

DEFINE_VECTOR_OBJECT(CTest);

int main()
{
	CTestps testps;
	printf("Create objects... \n");
	for ( int i=0; i < 10; i++)
	{
		CTest* ptest = new CTest();
		ptest->id = i;
		InsertObject(testps, ptest);
		if (FindObject(testps, ptest))
		{
			printf("Find OK!\n");
		}
		
	}

	printf("Delete objects... \n");
	START_OBJECTS_ITERATOR(CTestp, testps, testp);
	Delete(testp);
	END_OBJECTS_ITERATOR();

	return 0;
}