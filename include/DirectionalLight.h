#ifndef _DIRECTIONAL_LIGHT_
#define _DIRECTIONAL_LIGHT_

#include "Light.h"
#include "Vec3.h"

namespace target{
	class DirectionalLight : public Light{
		private:
			Vec3 direction;
		public:
			DirectionalLight(const LightType & type, std::string & name, const Vec3 & intensity, const Vec3 & direction) 
			: direction{direction}, Light(type, name, intensity){}
			inline Vec3 get_direction(){ return direction; }
	};
}

#endif