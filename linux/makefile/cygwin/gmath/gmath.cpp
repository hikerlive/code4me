#include <stdio.h>
#include "math.h"
#include "xstring.h"

int main(int argc, char* argv[])
{
    int i = 10,j=5;
    int sum = add(i, j);   
    printf("add(%d, %d) = %d\n", i, j, sum);


    gutils::XString str("hello world");
    printf("XString::data %s\n", str.GetData());
    return 0;
}
