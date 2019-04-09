#include "OrthoCamera.h"

target::Ray target::OrthoCamera::generate_ray(int x, int y){
	float nx = 1.f,ny = 1.f;
	float u = l + (r - l) * (x + 0.5) / nx;
	float v = b + (t - b) * (y + 0.5) / ny;

	Vec3 d = w * (-1);
	Point3d o = this->u * u + this->v * v + e;

	return Ray(o,d);
}