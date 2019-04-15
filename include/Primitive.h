#ifndef _PRIMITIVE_
#define _PRIMITIVE_

#include "Ray.h"
#include "SurfaceInteraction.h"
#include "Material.h"
#include <string>

namespace target{

	class SurfaceInteraction;
	class Primitive{
		protected:
			std::shared_ptr<Material> material;
			std::string name;

		public:
			Primitive(const std::string & name) : name{name}{}
			virtual bool intersect( const Ray& r, SurfaceInteraction *) const = 0;
			virtual bool intersect_p( const Ray& r ) const = 0;
			virtual const Material * get_material(void) const = 0;// { return material; }
		
	};

}

#endif