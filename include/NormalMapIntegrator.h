#ifndef _NORMAL_MAP_INTEGRATOR_
#define _NORMAL_MAP_INTEGRATOR_

#include "SampleIntegrator.h"

namespace target{
	class NormalMapIntegrator : public SampleIntegrator{
		public:
			NormalMapIntegrator( std::shared_ptr<Camera> & camera, const std::string & name, std::shared_ptr<Sampler> & sampler)
            : SampleIntegrator(camera, name, sampler){}
            NormalMapIntegrator() : SampleIntegrator(){}
			Color Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler, const int & depth = 0 ) override;
	};
}

#endif