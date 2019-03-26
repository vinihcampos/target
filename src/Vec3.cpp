#include "Vec3.h"

target::Vec3::Vec3(float x, float y, float z){
	point = Point3d(x,y,z);
}

target::Vec3::Vec3(const Point3d & point_){
	point = Point3d(point_);
}