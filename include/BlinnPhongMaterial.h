#ifndef _BLINN_MATERIAL_
#define _BLINN_MATERIAL_

#include "Material.h"

namespace target{

	class BlinnPhongMaterial : public Material{
		private:
			Color ambient;
			Color specular;
			double glossiness; 
		public:
			BlinnPhongMaterial(const Color & difuse, 
							   const Color & ambient, 
							   const Color & specular, 
							   const double & glossiness) 
			: ambient{ambient}, specular{specular}, 
			glossiness{glossiness}, Material{difuse}{}

			inline Color kd(){ return this->difuse; }
			inline Color ka(){ return ambient; }
			inline Color ks(){ return specular; }
			inline double gloss(){ return glossiness; }
	};

}

#endif