/**
 * Graphics.h
 */

class Graphics {
public:
	Graphics();
	~Graphics();
		
	void drawLine(int x0, int y0, int x1, int y1);
	void drawLine(Point p0, Point p1);
	
	void drawRectangle(int x0, int y0, int x1, int y1);
	void drawRectangle(Point p0, Point p1);

	void drawArc(int x, int y, int r);
	void drawArc(Point p, int r);

private:
	class Impl;
	Impl* impl;
};
