#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
    using namespace std;

    string str("Hello World");
    string::iterator itr;
    str.erase(str.length()-1, 1);
    printf("%s", str.c_str());
    return 0;
}
