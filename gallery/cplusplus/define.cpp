// define.cpp

#define max(a,b) ((a)>(b)?(a):(b))

int main(int argc, char** argv)
{
	int m = 5, z=0;
	max(++m, z);
	max(++m, z+10);
	cout << m << z << endl;
	return 0;
}