#ifndef _RAY_
#define _RAY_

#include "Point3.h"
#include "Vec3.h"

namespace target{

	class Ray{
		private:
			Point3 o;
			Vec3 d;
			
		public:			
			double tMax;
			
			Ray() : tMax{std::numeric_limits<double>::max()} {}
			Ray(const Point3 o, const Vec3 d) : o{o}, d{d}, tMax{std::numeric_limits<double>::max()} {}

			inline Point3 operator()(const float & t) { return d * t + o; }
			inline Point3 getOrigin() const { return o; }
			inline Vec3 getDirection() const { return d; }

			inline bool operator==(const Ray & r) const{
				return (d == r.getDirection() && o == r.getOrigin());
			}

			friend inline std::ostream& operator<<(std::ostream &os, const Ray &r){
				os << "r=[" << r.getOrigin() << "," << r.getDirection() << "]";
				return os;
 			}
	};

}

#endif