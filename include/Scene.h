#ifndef _SCENE_
#define _SCENE_

#include "Light.h"
#include "Background.h"
#include "Ray.h"
#include "SurfaceInteraction.h"
#include "Primitive.h"
#include <vector>
#include <memory>

namespace target{
	class Scene{
	    public:
	        std::vector<std::shared_ptr<Light>> lights; // list of lights
	        std::shared_ptr< Background > background; // The background object.
			
			Scene( std::shared_ptr<Primitive> pr, const std::vector<std::shared_ptr<Light>>& ls )
	            : lights{ls}, primitives{pr} { }
	        
	        bool intersect( const Ray& r, SurfaceInteraction *isect ) const;
	        
	        bool intersect_p( const Ray& r ) const;
	    
	    private:
	        std::shared_ptr<Primitive> primitives; 
	};
}

#endif