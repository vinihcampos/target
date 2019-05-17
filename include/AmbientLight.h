#ifndef _AMBIENT_LIGHT_
#define _AMBIENT_LIGHT_

#include "Light.h"

namespace target{
	class AmbientLight : public Light{
		public:
			AmbientLight(const LightType & type, std::string & name, const Vec3 & intensity) 
			: Light(type, name, intensity){}			
	};
}

#endif