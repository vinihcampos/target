#ifndef _CAMERA_
#define _CAMERA_

#include "Vec3.h"
#include "Ray.h"

namespace target{
	class Camera{
		
		protected:
			Vec3 w, u, v;
			Point3d e;
			float l,r,b,t;
			int nx, ny;

		public:
			Camera(){}
			Camera(Vec3 look_from, Vec3 look_at, Vec3 vup, int nx, int ny, float l, float r, float b, float t)
				: nx{nx}, ny{ny}, l{l}, r{r}, b{b},t{t} {

				Vec3 gaze = (look_at-look_from)*(-1);

				w = gaze.norm();
				u = vup.cross(w).norm();
				v = w.cross(u).norm();
				e = Point3d( look_from.x(),look_from.y(),look_from.z() );

				//std::cout << u << std::endl;
				//std::cout << v << std::endl;
				//std::cout << w << std::endl;
			}
			virtual Ray generate_ray(int x, int y) = 0;	
	};
}

#endif