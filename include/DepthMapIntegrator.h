#ifndef _DEPTH_MAP_INTEGRATOR_
#define _DEPTH_MAP_INTEGRATOR_

#include "SampleIntegrator.h"
#include "Material.h"
#include <limits>

namespace target{
	class DepthMapIntegrator : public SampleIntegrator{
		private:
			float zmin = std::numeric_limits<double>::infinity();
			float zmax = -std::numeric_limits<double>::infinity();

			std::shared_ptr<Material> near;
			std::shared_ptr<Material> far;
		public:
			DepthMapIntegrator( std::shared_ptr<Camera> & camera, std::shared_ptr<Material> & far, std::shared_ptr<Material> & near, std::shared_ptr<Sampler> & sampler)
            : far{far}, near{near}, SampleIntegrator(camera, sampler){}
			Color Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler ) override;
			void preprocess( const Scene& scene ) override;
	};
}

#endif