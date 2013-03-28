/**
 * 2012-04-26
 *
 * non-virtual function interface method
 *
 * 解决二进制文件新版本或者接口添加的兼容问题：有几种方式:
 * 1.COM也解决了二进制接口兼容的问题,如：IGraphics, IGraphics2, IGraphics3。
 *   a.以多重继承的方式来实现,Graphics : public IGrapics, IGraphic2, IGraphic3。
 *   b.仍是以virtual function interface method的方式来实现。
 * 
 * 2.以本示例代码中提供的non-virtual function interface method的方式来实现。
 *   a.头文件提供一个包装类函数声明和实现指针。
 *   b.若要增加新接口时，只要在类函数声明部分添加新函数，添加显示forward调用即可。
 *
 *原因分析：
 *  1.虚函数本质问题在于C++以vtable[offset]方式来实现虚函数调用。
 *  2.offset又是根据虚函数声明的位置隐式确定的。
 *  3.添加新接口会影响虚函数声明顺序，如+部分的示例，新库不能兼容老库，因为函数offset发生变化啦。
 *
 *结论：
 *  1.不以virtual function method来实现动态链接库
 *  2.要用看库的使用范围，如团队内部小范围使用，若要提供给客户，其它团队，则视情况而定。
 *
 */

#include "non_virtual_function.h"

class Graphics::Impl {
public:
	void drawLine(int x0, int y0, int x1, int y1);
	void drawLine(double x0, double y0, double x1, double y1);
	void drawLine(Point p0, Point p1);

	void drawRectangle(int x0, int y0, int x1, int y1);
	void drawRectangle(Point p0, Point p1);

	void drawArc(int x, int y, int r);
	void drawArc(Point p, int r);
};

Graphics::Graphics() : impl(new Impl) 
{
}

Graphics::~Graphics() 
{
	delete impl;
}

void Graphics::drawLine(int x0, int y0, int x1, int y1) 
{
	impl->drawLine(x0, y0, x1, y1);
}
void Graphics::drawLine(Point p0, Point p1)
{
	impl->drawLine(p0, p1);
}
