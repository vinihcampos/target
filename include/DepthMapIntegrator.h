#ifndef _DEPTH_MAP_INTEGRATOR_
#define _DEPTH_MAP_INTEGRATOR_

#include "SampleIntegrator.h"
#include "Material.h"
#include <limits>

namespace target{
	class DepthMapIntegrator : public SampleIntegrator{
		private:
			double zmin = std::numeric_limits<double>::infinity();
			double zmax = -std::numeric_limits<double>::infinity();

			std::shared_ptr<Material> near;
			std::shared_ptr<Material> far;
		public:
			DepthMapIntegrator( std::shared_ptr<Camera> & camera, const std::string & name,std::shared_ptr<Material> & far, std::shared_ptr<Material> & near, std::shared_ptr<Sampler> & sampler)
            : far{far}, near{near}, SampleIntegrator(camera, name, sampler){}
            DepthMapIntegrator(std::shared_ptr<Material> & far, std::shared_ptr<Material> & near) : far{far}, near{near}, SampleIntegrator(){}
			Color Li( const Ray& ray, const Scene& scene, Sampler& sampler, const int & depth = 0 ) override;
			void preprocess( const Scene& scene ) override;
	};
}

#endif