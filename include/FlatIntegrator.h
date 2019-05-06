#ifndef _FLAT_INTEGRATOR_
#define _FLAT_INTEGRATOR_

#include "SampleIntegrator.h"

namespace target{
	class FlatIntegrator : public SampleIntegrator{
		public:
			FlatIntegrator( std::shared_ptr<Camera> & camera, std::shared_ptr<Sampler> & sampler)
            : SampleIntegrator(camera, sampler){}
			Color Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler ) override;
	};
}

#endif