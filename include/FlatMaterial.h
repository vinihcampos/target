#ifndef _FLAT_MATERIAL_
#define _FLAT_MATERIAL_

#include "Material.h"

namespace target{

	class FlatMaterial : public Material{
		public:
			FlatMaterial(const Color & color) : Material{color}{}
			inline Color kd(){ return this->difuse; }
	};

}

#endif