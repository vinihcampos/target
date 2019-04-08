#ifndef _RAY_
#define _RAY_

#include "Point3d.h"
#include "Vec3.h"

namespace target{

	class Ray{
		private:
			Point3d o;
			Vec3 d;
		public:
			Ray(const Point3d& o, const Vec3& d) : o{o}, d{d} {}
			inline Point3d operator()(const float & t) { return d * t + o; }
			inline Point3d getOrigin() const { return o; }
			inline Vec3 getDirection() const { return d; }
	};

}

#endif