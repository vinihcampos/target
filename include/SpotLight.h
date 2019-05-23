#ifndef _SPOT_LIGHT_
#define _SPOT_LIGHT_

#include "Light.h"
#include "Point3.h"

namespace target{
	class SpotLight : public Light{
		private:
			Point3 position;
			Point3 point_at;
			double cutoff;
			double falloff;
		public:
			SpotLight(const LightType & type, std::string & name, const Vec3 & intensity, 
				const Point3 & position, const Point3 & point_at, const double & cutoff, const double & falloff) 
			: position{position}, point_at{point_at}, cutoff{cutoff}, falloff{falloff}, Light(type, name, intensity){}
			inline Point3 get_position(){ return position; }
			inline Point3 get_point_at(){ return point_at; }
			inline double get_cutoff(){ return cutoff; }
			inline double get_falloff(){ return falloff; }
	};
}

#endif