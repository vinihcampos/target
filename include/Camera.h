#ifndef _CAMERA_
#define _CAMERA_

#include "Point3.h"
#include "Point2.h"
#include "Vec3.h"
#include "Ray.h"
#include "Buffer.h"
#include <memory>
#include <cmath>
#include "Util.h"

namespace target{

	class Camera{
		
		protected:
			Vec3 w, u, v;
			Point3 e;
			double l,r,b,t;
			int nx, ny;

		public:
			std::shared_ptr<Buffer> buffer;
			Camera(){}
			Camera(std::shared_ptr<Buffer> & buffer, Vec3 look_from, Vec3 look_at, Vec3 vup, int nx, int ny, double l, double r, double b, double t)
				: buffer{buffer}, nx{nx}, ny{ny}, l{l}, r{r}, b{b}, t{t} {
				Vec3 gaze = (look_at-look_from)*(-1);

				w = gaze.norm();
				u = vup.cross(w).norm();
				v = w.cross(u).norm();
				e = Point3( look_from.x(),look_from.y(),look_from.z() );
			}
			virtual Ray generate_ray(int y, int x) = 0;
			inline Point2 generate_point(double u, double v){
				return Point2( (u + 1) / 2.0 * nx, v * ny );
			}
			inline void get_uv(const Vec3 & p, double & u, double & v){
				double phi = std::atan(p.z()/ p.x());
				double theta = std::acos((p.y() * (-1)) / p.length());
				u = phi / (2 * M_PI) ;
				v = theta / M_PI;
			}
	};
}

#endif