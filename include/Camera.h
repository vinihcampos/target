#ifndef _CAMERA_
#define _CAMERA_

#include "Vec3.h"
#include "Ray.h"

namespace target{
	class Camera{
		
		protected:
			Vec3 w, u, v;
			Point3d e;

		public:
			Camera(Vec3 look_at, Vec3 look_from, Vec3 vup){
				w = ((look_at-look_from)*(-1)).norm();
				u = vup.cross(w).norm();
				v = w.cross(u).norm();
				e = Point3d( look_from.x(),look_from.y(),look_from.z() );
			}
			virtual Ray generate_ray(int x, int y) = 0;	
	};
}

#endif