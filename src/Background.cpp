#include "Background.h"

target::Color target::Background::interpolate(const Buffer & buffer, const Point3d p){

	int x1 = 0;
	int x2 = buffer.getWidth() - 1;
	int y1 = buffer.getHeight() - 1;
	int y2 = 0;

	float ratio = 1.0 / ( (x2-x1) * (y2-y1) * 1.0);

	Color tl, bl, tr, br;
	tl = buffer.getTl();
	bl = buffer.getBl();
	tr = buffer.getTr();
	br = buffer.getBr();

	int productRed = bl.r()*(x2-p.x)*(y2-p.y) + br.r()*(p.x-x1)*(y2-p.y) + tl.r()*(x2-p.x)*(p.y-y1) + tr.r()*(p.x-x1)*(p.y-y1);
	int productGreen = bl.g()*(x2-p.x)*(y2-p.y) + br.g()*(p.x-x1)*(y2-p.y) + tl.g()*(x2-p.x)*(p.y-y1) + tr.g()*(p.x-x1)*(p.y-y1);
	int productBlue = bl.b()*(x2-p.x)*(y2-p.y) + br.b()*(p.x-x1)*(y2-p.y) + tl.b()*(x2-p.x)*(p.y-y1) + tr.b()*(p.x-x1)*(p.y-y1);

	return Color(ratio*productRed, ratio*productGreen, ratio*productBlue);
}