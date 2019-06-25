#ifndef _AMBIENT_LIGHT_
#define _AMBIENT_LIGHT_

#include "Light.h"

namespace target{

	/**
	* Provides methods to create ambient lights.
	* 
	* @author Vinícius Campos
	* @date 5/17/2019
	*/
	class AmbientLight : public Light{
		public:
			/**
			*	Creates an ambient light.
			*	@param type type of the light.
			*	@param name name of the light.
			*	@param intensity intensity of the light.
			*/
			AmbientLight(const LightType & type, std::string & name, const Vec3 & intensity) 
			: Light(type, name, intensity){}			
	};
}

#endif