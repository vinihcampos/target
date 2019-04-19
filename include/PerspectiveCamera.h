#ifndef _PERSPECTIVE_
#define _PERSPECTIVE_

#include "Ray.h"
#include "Camera.h"
#include <math.h>

namespace target{

	const double PI = 3.14159265;

	class PerspectiveCamera : public Camera{
		private:
			double fovy, fd, aspect;
		public:
			PerspectiveCamera(Vec3 look_from, Vec3 look_at, Vec3 vup, int nx, int ny, double fovy, double fd, double aspect)
			: fovy{fovy}, fd{fd}, aspect{aspect}, 
			  Camera(look_from, look_at, vup, nx, ny, 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd * aspect * (-1), 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd * aspect, 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd * (-1), 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd){}
			Ray generate_ray(int x, int y) override;	
	};
}

#endif