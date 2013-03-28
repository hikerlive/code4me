// bitflag.cpp
// Demonstrate C++ binary operation

#include <stdio.h>

#define BIT_LSHIFT(v, n)	((v) << (n))
#define BIT_RSHIFT(v, n)	((v) >> (n))
#define BIT_FLAG(n)		BIT_LSHIFT(1, n)
#define BIT_CHECK_FLAG(m, n)	((m & BIT_FLAG(n))?1:0)
#define BIT_SET_FLAG(m, n)	(m |= BIT_FLAG(n))
#define BIT_CLEAR_FLAG(m, n)	(m &= ~BIT_FLAG(n))


// 位段
// 总长为32位,4个字节
struct packed_data
{
	unsigned a:2; // 占2位
	unsigned b:6; // 占6位
	unsigned c:4;
	unsigned d:4;
	int i;
}data;


//关于位段的定义和引用,有几点说明
/*
 (1)位段成员的类型必须指定为unsigned int 类型
 (2)若某一位段要从另一个字开始存放.可以用以下形式定义:
	unsigned a : 1;(一个存储单元)
	unsigned b : 2;()
	unsigned   : 0;
	unsigned c : 3 (另一个存储单元)

 (3)一个位段必须存储在同一存储单元中,不能跨两个单元.如果第一个单元空间不能容纳
    下一个位段,则该空间不用,而从下一个单元起存放该位段。

 (4)可以定义无名字段
    unsigned a : 1;
	unsigned b : 2
	unsigned   : 3; (这两位空间不用)
	unsigned c : 4;

 (5)位段可以用整型格式符输出
    printf("%d,%d,%d",data.a, data.b,data.c)

 (6)位段可以在数值表达式中引用，它会被系统自动地转换成整型数。
    data.a + 5/data.b
*/


int main(int argc, char** argv)
{
	int i =1;
	i = BIT_LSHIFT(i,10);
	printf("left shift %d\n", i);

	int k= BIT_FLAG(-2);
	printf("bit flag %d\n", k);

	data.a = 2; // 注意位段a所表示值最大范围.
	data.a = 8; // error:此时data.a=0;取得是8的低2位1000.

	data.i = 1<<20;
	printf("%d,%d,%d,%d,%d\n", data.a, data.b, data.c, data.d, data.i);

	// 两个位段区域为2个字节,i为4个字节.总体按说应该是6个字节.
	// 但考虑到字节对齐等因素,输出的是8个字节.
	printf("data size: %d\n", sizeof(data)); // surprise: size = 8;

	return 0;
}
