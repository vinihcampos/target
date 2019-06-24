#ifndef _PRIMITIVE_
#define _PRIMITIVE_

#include "Ray.h"
#include "SurfaceInteraction.h"
#include "Material.h"
#include "Bounds3.h"
#include "Transform.h"
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

			virtual bool intersect( Ray& r, SurfaceInteraction *) const = 0;
			virtual bool intersect_p( const Ray& r ) const = 0;
			virtual bool intersect_p( const Ray& r, double tmin, double tmax ) const = 0;
			virtual Bounds3 get_bounding_box() const /*{ std::cout << "AA\n"; return box; }*/ = 0;
	};

	class Shape : public Primitive{
		protected:
			std::string name;
			std::shared_ptr<Transform> transform;
		public:
			Shape(const std::string & name) : name{name} {
				transform = std::shared_ptr<Transform>(new Transform());
			}
			
			Shape(const std::string & name, const std::shared_ptr<Transform> & transform) : name{name}, transform{transform} {}
			
			inline std::string getName(){ return name; }

			virtual bool intersect( Ray& r, SurfaceInteraction *) const = 0;
			virtual bool intersect_p( const Ray& r ) const = 0;
			virtual bool intersect_p( const Ray& r, double tmin, double tmax ) const = 0;
			virtual Bounds3 get_bounding_box() const = 0;
	};
}

#endif