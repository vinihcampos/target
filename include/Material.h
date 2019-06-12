#ifndef _MATERIAL_
#define _MATERIAL_

namespace target{

	class Material{
		protected:
			Color difuse;
		public:
			Material(const Color & difuse) : difuse{difuse}{}
			virtual ~Material() = default;
			inline Color kd(){ return this->difuse; }
	};

}

#endif