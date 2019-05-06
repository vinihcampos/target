#ifndef _FLAT_MATERIAL_
#define _FLAT_MATERIAL_

#include "Material.h"

namespace target{

	class FlatMaterial : public Material{
		public:
			inline Color kd(){ return this->color; }
	};

}

#endif