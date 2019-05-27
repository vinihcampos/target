#ifndef _PRIMITIVE_
#define _PRIMITIVE_

#include "Ray.h"
#include "SurfaceInteraction.h"
#include "Material.h"
#include "Bounds3.h"
#include <string>

namespace target{

	class SurfaceInteraction;
	class Primitive{
		protected:
			std::shared_ptr<Material> material;
			std::string name;
			Bounds3 box;

		public:
			Primitive(const std::string & name) : name{name}{}
			virtual bool intersect( Ray& r, SurfaceInteraction *) const = 0;
			virtual bool intersect_p( const Ray& r ) const = 0;
			virtual bool intersect_p( const Ray& r, double tmin, double tmax ) const = 0;
			Material * get_material(void) { return material.get(); }
			void set_material(std::shared_ptr<Material> & material){ this->material = material; }
			inline std::string getName(){ return name; }
			inline Bounds3 get_bounding_box(){ return box; }
	};
	typedef Primitive Shape;
}

#endif