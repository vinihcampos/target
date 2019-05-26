#ifndef _BLINN_INTEGRATOR_
#define _BLINN_INTEGRATOR_

#include "SampleIntegrator.h"
#include "Vec3.h"

namespace target{
	class BlinnPhongIntegrator : public SampleIntegrator{
		public:
			BlinnPhongIntegrator( std::shared_ptr<Camera> & camera, 
				const std::string & name, std::shared_ptr<Sampler> & sampler)
            : SampleIntegrator(camera, name, sampler){}
            BlinnPhongIntegrator() : SampleIntegrator(){}
			Color Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler, const int & depth = 0 ) override;
			inline Color reflect(Color & intensity, Vec3 & n){
				return intensity - (n * (2 * n.dot(intensity)));
			}
	};
}

#endif