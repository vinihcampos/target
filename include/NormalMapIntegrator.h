#ifndef _NORMAL_INTEGRATOR_
#define _NORMAL_INTEGRATOR_

#include "SampleIntegrator.h"

namespace target{
	class NormalIntegrator : public SampleIntegrator{
		public:
			NormalIntegrator( std::shared_ptr<Camera> & camera, std::shared_ptr<Sampler> & sampler)
            : SampleIntegrator(camera, sampler){}
			Color Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler ) override;
	};
}

#endif