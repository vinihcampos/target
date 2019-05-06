#ifndef _MATERIAL_
#define _MATERIAL_

namespace target{

	class Material{
		protected:
			Color color;
		public:
			Material(const Color & color) : color{color}{}
			virtual ~Material() = default;
	};

}

#endif