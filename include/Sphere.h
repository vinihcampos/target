#ifndef _SPHERE_
#define _SPHERE_

#include "Primitive.h"
#include "Vec3.h"
#include <string>
#include <cmath>

namespace target{

	class Sphere : public Shape{
		private:
			Vec3 center;
			float radius;

		public:
			Sphere(const Vec3 & center, const float & radius, const std::string & name ) 
				: Shape(name), center{center}, radius{radius}{
					this->box = Bounds3( this->center - radius, this->center + radius );
				}
			inline bool intersect( Ray & r, SurfaceInteraction * surface = nullptr) const { 
				
				Vec3 origin = Vec3(r.getOrigin());
				Vec3 OC = origin - center;

				float a = r.getDirection().dot(r.getDirection());
				float b = 2 * r.getDirection().dot( OC );
				float c = OC.dot(OC) - (radius*radius);

				float delta = b*b - 4*a*c;
				if(delta <= 0)
					return false;

				float r1 = (-b + std::sqrt(delta)) / (2 * a);
				float r2 = (-b - std::sqrt(delta)) / (2 * a);
				float min_root = std::min(r1,r2);
				float max_root = std::max(r1,r2);

				if(surface != nullptr){
					surface->time = min_root;
					surface->p = r(min_root);
					surface->p_max = r(max_root);
					surface->n = (Vec3(surface->p) - center) * 2.0;
					surface->wo = (origin - r.getDirection());					
				}

				return true; 
			}
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
			inline bool intersect_p( const Ray& r, double tmin, double tmax ) const{
				Vec3 origin = Vec3(r.getOrigin());
				Vec3 OC = origin - center;

				float a = r.getDirection().dot(r.getDirection());
				float b = 2 * r.getDirection().dot( OC );
				float c = OC.dot(OC) - (radius*radius);

				float delta = b*b - 4*a*c;
				if(delta <= 0)
					return false;

				float r1 = (-b + std::sqrt(delta)) / (2 * a);
				if((r1 >= tmin) && (r1 <= tmax)) return true;
				
				float r2 = (-b - std::sqrt(delta)) / (2 * a);
				return ((r2 >= tmin) && (r2 <= tmax));
			}
			inline const Material * get_material(void) const { return this->material.get(); }
			inline Vec3 getCenter(){ return center; }
			inline float getRadius() { return radius; }
	};

}

#endif