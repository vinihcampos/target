#ifndef _BLINN_INTEGRATOR_
#define _BLINN_INTEGRATOR_

#include "SampleIntegrator.h"
#include "Vec3.h"
#include "Bounds3.h"

namespace target{
	class BlinnPhongIntegrator : public SampleIntegrator{
		private:
			Bounds3 big_box;
			double worldRadius = 0;
		public:
			BlinnPhongIntegrator( std::shared_ptr<Camera> & camera, 
				const std::string & name, std::shared_ptr<Sampler> & sampler)
            : SampleIntegrator(camera, name, sampler){}
            BlinnPhongIntegrator() : SampleIntegrator(){}
			Color Li( const Ray& ray, const Scene& scene, Sampler& sampler, const int & depth = 0 ) override;
			void preprocess( const Scene& scene ) override;

			inline Color reflect(Color intensity, Vec3 n){
				return intensity - (n * (2 * intensity.dot(n)));
			}
	};
}

#endif