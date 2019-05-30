#ifndef _FLAT_INTEGRATOR_
#define _FLAT_INTEGRATOR_

#include "SampleIntegrator.h"

namespace target{
	class FlatIntegrator : public SampleIntegrator{
		public:
			FlatIntegrator( std::shared_ptr<Camera> & camera, const std::string & name, std::shared_ptr<Sampler> & sampler)
            : SampleIntegrator(camera, name, sampler){}
            FlatIntegrator() : SampleIntegrator(){}
			Color Li( const Ray& ray, const Scene& scene, Sampler& sampler, const int & depth = 0 ) override;
	};
}

#endif