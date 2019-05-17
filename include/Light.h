#ifndef _LIGHT_
#define _LIGHT_

#include "Vec3.h"
#include <string>

namespace target{

	enum LightType{
		AMBIENT,
		POINT,
		DIRECTIONAL,
		SPOT 	
	};

	class Light{
		protected:
			Vec3 intensity;
			LightType type;
			std::string name;
		public:
			Light(const LightType & type, std::string & name, const Vec3 & intensity) 
			: type{type}, name{name}, intensity{intensity}{}
			inline Vec3 get_intensity(){ return intensity; }
			inline LightType get_type(){ return type; }
			virtual ~Light() = default;
	};
}

#endif