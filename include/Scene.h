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
			std::vector<std::shared_ptr<Primitive>> primitives;

			Scene( std::vector<std::shared_ptr<Primitive>> prs, const std::vector<std::shared_ptr<Light>>& ls )
	            : lights{ls}, primitives{prs} { }

	        Scene( std::vector<std::shared_ptr<Primitive>> prs)
	            : primitives{prs} { }
	        
	        bool intersect( Ray & r, SurfaceInteraction *isect ) const{
	        	for(std::shared_ptr<Primitive> pr : primitives){
	        		if(pr->intersect(r, isect)){
	        			return true;
	        		}
	        	}
	        	return false;
	        }
	        
	        bool intersect_p( const Ray& r, std::shared_ptr<Primitive> & primitive) const {

	        	for(std::shared_ptr<Primitive> pr : primitives){
	        		if(pr->intersect_p(r)){
	        			primitive = pr;
	        			return true;
	        		}
	        	}
	        	return false;
	        }
	};
}

#endif