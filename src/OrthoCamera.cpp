#include "OrthoCamera.h"
#include "Point2.h"

target::Ray target::OrthoCamera::generate_ray(int y, int x){
	double u = l + (r - l) * (x + 0.5) / this->nx;
	double v = b + (t - b) * (y + 0.5) / this->ny;

	Vec3 d = this->w * (-1);
	Point3 o = this->u * u + this->v * v + this->e;

	return Ray(o,d);
}