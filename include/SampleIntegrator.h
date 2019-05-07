#ifndef _SAMPLE_INTEGRATOR_
#define _SAMPLE_INTEGRATOR_

#include "Integrator.h"
#include "Camera.h"
#include "Sampler.h"
#include "Vec3.h"
#include "Scene.h"
#include <memory>

namespace target{
	class SampleIntegrator : public Integrator{

	    protected:
	        std::shared_ptr<Camera> camera;
	        std::shared_ptr<Sampler> sampler;

		public:
			SampleIntegrator( std::shared_ptr<Camera> & camera, std::shared_ptr<Sampler> & sampler)
            : camera{camera}, sampler{sampler}{}
			virtual void preprocess( const Scene& scene ){}
	        void render( const Scene& scene );
			virtual Color Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler ) = 0;
		
	};
}

#endif