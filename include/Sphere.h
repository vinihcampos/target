#ifndef _SPHERE_
#define _SPHERE_

#include "Primitive.h"
#include "Vec3.h"
#include <string>

namespace target{

	class Sphere : public Shape{
		private:
			Vec3 center;
			float radius;

		public:
			Sphere(const Vec3 & center, const float & radius, const std::string & name ) 
				: Shape(name), center{center}, radius{radius}{}
			inline bool intersect( const Ray& r, SurfaceInteraction *) const { return true; }
			inline bool intersect_p( const Ray& r ) const{
				
				// (P-C).(P-C) = r²
				// ((O+tD-C).(O+tD-C) = r²
				// ((tD+(O-C)).(tD+(O-C)) = r²
				// t²(D.D) + 2t(D.(O-C)) + ((O-C).(O-C)) = r²			

				Vec3 origin = Vec3(r.getOrigin());
				Vec3 OC = origin - center;

				float a = r.getDirection().dot(r.getDirection());
				float b = 2 * r.getDirection().dot( OC );
				float c = OC.dot(OC) - (radius*radius);

				float delta = b*b - 4*a*c;
				return (delta > 0);

			}
			inline const Material * get_material(void) const { return this->material.get(); }
			inline Vec3 getCenter(){ return center; }
			inline float getRadius() { return radius; }
	};

}

#endif