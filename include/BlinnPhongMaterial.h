#ifndef _BLINN_MATERIAL_
#define _BLINN_MATERIAL_

#include "Material.h"

namespace target{

	class BlinnPhongMaterial : public Material{
		private:
			Color ambient;
			Color specular;
			Color mirror;
			double glossiness;
		public:
			BlinnPhongMaterial(const Color & difuse, 
							   const Color & ambient, 
							   const Color & specular, 
							   const double & glossiness,
							   const Color & mirror) 
			: ambient{ambient}, specular{specular}, 
			glossiness{glossiness}, mirror{mirror}, Material{difuse}{}

			inline Color kd(){ return this->difuse; }
			inline Color ka(){ return ambient; }
			inline Color ks(){ return specular; }
			inline Color km(){ return mirror; }
			inline double gloss(){ return glossiness; }
	};

}

#endif