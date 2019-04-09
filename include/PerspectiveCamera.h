#ifndef _PERSPECTIVE_
#define _PERSPECTIVE_

#include "Ray.h"
#include "Camera.h"
#include <math.h>

namespace target{

	const float PI = 3.14159265;

	class PerspectiveCamera : public Camera{
		private:
			float fovy, fd, aspect;
		public:
			PerspectiveCamera(Vec3 look_at, Vec3 look_from, Vec3 vup, int nx, int ny, float fovy, float fd, float aspect)
			: fovy{fovy}, fd{fd}, aspect{aspect}, 
			  Camera(look_at, look_from, vup, nx, ny, 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd * aspect * (-1), 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd * aspect, 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd * (-1), 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd){}
			Ray generate_ray(int x, int y) override;	
	};
}

#endif