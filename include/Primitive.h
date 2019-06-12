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
			Bounds3 box;

		public:
			Material * get_material(void) { return material.get(); }
			std::shared_ptr<Material>  get_material_shared(void) { return material; }
			void set_material(const std::shared_ptr<Material> & material){ this->material = material; }
			inline Bounds3 get_bounding_box(){ return box; }

			virtual bool intersect( Ray& r, SurfaceInteraction *) const = 0;
			virtual bool intersect_p( const Ray& r ) const = 0;
			virtual bool intersect_p( const Ray& r, double tmin, double tmax ) const = 0;
	};

	class Shape : public Primitive{
		protected:
			std::string name;
		public:
			Shape(const std::string & name) : name{name} {}
			inline std::string getName(){ return name; }

			virtual bool intersect( Ray& r, SurfaceInteraction *) const = 0;
			virtual bool intersect_p( const Ray& r ) const = 0;
			virtual bool intersect_p( const Ray& r, double tmin, double tmax ) const = 0;
	};
}

#endif