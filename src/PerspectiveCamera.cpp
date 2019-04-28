#include "PerspectiveCamera.h"

target::Ray target::PerspectiveCamera::generate_ray(int x, int y){
	double u = l + (r - l) * (y + 0.5) / (1.0 * this->nx);
	double v = b + (t - b) * (x + 0.5) / (1.0 * this->ny);

	Vec3 d = this->w * (-fd) + this->u * u + this->v * v;
	Point3 o = this->e;

	return Ray(o,d.norm());
}