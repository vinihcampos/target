#ifndef _VEC3_
#define _VEC3_

#include "Point3d.h"

namespace target{
	
	class Vec3{
		private:
			Point3d point;

		public:
			Vec3(float x = 0, float y = 0, float z = 0);
			Vec3(const Point3d & point_);

			inline float x(){ return point.x; }
			inline float y(){ return point.y; }
			inline float z(){ return point.z; }

		};

}

#endif