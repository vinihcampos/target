#include "PerspectiveCamera.h"

target::Ray target::PerspectiveCamera::generate_ray(int x, int y){
	float u = l + (r - l) * (y + 0.5) / this->nx;
	float v = b + (t - b) * (x + 0.5) / this->ny;

	Vec3 d = this->w * (-fd) + this->u * u + this->v * v;
	Point3d o = this->e;

	return Ray(o,d.norm());
}