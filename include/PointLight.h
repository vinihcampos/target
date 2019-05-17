#ifndef _POINT_LIGHT_
#define _POINT_LIGHT_

#include "Light.h"
#include "Point3.h"

namespace target{
	class PointLight : public Light{
		private:
			Point3 position;
		public:
			PointLight(const LightType & type, std::string & name, const Vec3 & intensity, const Point3 & position) 
			: position{position}, Light(type, name, intensity){}
			inline Point3 get_position(){ return position; }
	};
}

#endif