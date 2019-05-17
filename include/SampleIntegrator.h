#ifndef _SAMPLE_INTEGRATOR_
#define _SAMPLE_INTEGRATOR_

#include "Integrator.h"
#include "Camera.h"
#include "Sampler.h"
#include "Vec3.h"
#include "Scene.h"
#include <string>
#include <memory>

namespace target{
	class SampleIntegrator : public Integrator{

	    protected:
	        std::shared_ptr<Camera> camera;
	        std::shared_ptr<Sampler> sampler;
	        std::string name;

		public:
			SampleIntegrator( std::shared_ptr<Camera> & camera, const std::string & name, std::shared_ptr<Sampler> & sampler)
            : camera{camera}, name{name}, sampler{sampler}{}
            SampleIntegrator(){}
            inline void set_camera(std::shared_ptr<Camera> & camera_){ camera = camera_; }
            inline void set_name(const std::string & name_){ name = name_; }
            inline void set_sampler(std::shared_ptr<Sampler> & sampler_){ sampler = sampler_; }
	        void render( const Scene& scene );
			virtual void preprocess( const Scene& scene ){}
			virtual Color Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler ) = 0;
		
	};
}

#endif