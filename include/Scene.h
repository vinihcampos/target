#ifndef _SCENE_
#define _SCENE_

#include "Light.h"
#include "Background.h"
#include "Ray.h"
#include "SurfaceInteraction.h"
#include "Primitive.h"
#include <limits>
#include <vector>
#include <memory>

namespace target{
	class Scene{
	    public:
	        std::vector<std::shared_ptr<Light>> lights;
	        std::shared_ptr< Background > background;
			std::vector<std::shared_ptr<Primitive>> primitives;

			Scene( std::vector<std::shared_ptr<Primitive>> prs, const std::vector<std::shared_ptr<Light>>& ls )
	            : lights{ls}, primitives{prs} { }

	        Scene( std::vector<std::shared_ptr<Primitive>> prs)
	            : primitives{prs} { }
	        
	        bool intersect( Ray & r, SurfaceInteraction *isect ) const{
	        	double min_p = std::numeric_limits<double>::max();
	        	bool hitted = false;
	        	for(std::shared_ptr<Primitive> pr : primitives){
	        		if(pr->intersect(r, isect)){
	        			if(isect->time < min_p){
	        				isect->primitive = pr;
	        				min_p = isect->time;
	        			}
	        			hitted = true;
	        		}
	        	}
	        	return hitted;
	        }
	        
	        bool intersect_p( const Ray& r) const {
	        	for(std::shared_ptr<Primitive> pr : primitives){
	        		if(pr->intersect_p(r)){
	        			return true;
	        		}
	        	}
	        	return false;
	        }

	        bool intersect_p( const Ray& r, double tmin, double tmax) const {
	        	for(std::shared_ptr<Primitive> pr : primitives){
	        		if(pr->intersect_p(r, tmin, tmax)){
	        			return true;
	        		}
	        	}
	        	return false;
	        }
	};
}

#endif