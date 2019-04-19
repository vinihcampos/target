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
			Ray(){}
			Ray(const Point3d o, const Vec3 d) : o{o}, d{d} {}
			inline Point3d operator()(const float & t) { return d * t + o; }
			inline Point3d getOrigin() const { return o; }
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