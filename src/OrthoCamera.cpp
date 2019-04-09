#include "OrthoCamera.h"

target::Ray target::OrthoCamera::generate_ray(int x, int y){
	float u = l + (r - l) * (y + 0.5) / this->nx;
	float v = b + (t - b) * (x + 0.5) / this->ny;

	Vec3 d = this->w * (-1);
	Point3d o = this->u * u + this->v * v + this->e;

	return Ray(o,d);
}