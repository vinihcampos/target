#include "OrthoCamera.h"

target::Ray target::OrthoCamera::generate_ray(int x, int y){
	double u = l + (r - l) * (y + 0.5) / this->nx;
	double v = b + (t - b) * (x + 0.5) / this->ny;

	Vec3 d = this->w * (-1);
	Point3 o = this->u * u + this->v * v + this->e;

	return Ray(o,d);
}