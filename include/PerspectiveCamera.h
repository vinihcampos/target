#ifndef _PERSPECTIVE_
#define _PERSPECTIVE_

#include "Ray.h"
#include "Camera.h"
#include "Buffer.h"
#include <math.h>

namespace target{

	class PerspectiveCamera : public Camera{
		private:
			double fovy, fd, aspect;
		public:
			PerspectiveCamera(std::shared_ptr<Buffer> & buffer, Vec3 look_from, Vec3 look_at, Vec3 vup, int nx, int ny, double fovy, double fd, double aspect)
			: fovy{fovy}, fd{fd}, aspect{aspect}, 
			  Camera(buffer, look_from, look_at, vup, nx, ny, 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd * aspect * (-1), 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd * aspect, 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd * (-1), 
			  std::tan((fovy/2.0)*(PI/180.0)) * fd){}
			Ray generate_ray(int y, int x) override;
	};
}

#endif