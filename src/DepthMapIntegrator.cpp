#include "DepthMapIntegrator.h"
#include "SurfaceInteraction.h"
#include "FlatMaterial.h"
#include "Point2.h"
#include "Vec3.h"
#include <cmath>

target::Color target::DepthMapIntegrator::Li( const Ray& ray, const Scene& scene, Sampler& sampler, const int & depth ){

    SurfaceInteraction *isect = new SurfaceInteraction();
    Ray r = ray;
    if (scene.intersect(r, isect)) {
        double t = isect->time;
        double interpolation = (t - zmin) / (zmax - zmin);

        FlatMaterial * farm  = static_cast<FlatMaterial*>(far.get());
        FlatMaterial * nearm = static_cast<FlatMaterial*>(near.get());

        Color c( interpolation * (farm->kd().r()-nearm->kd().r()) + nearm->kd().r(),
        		 interpolation * (farm->kd().g()-nearm->kd().g()) + nearm->kd().g(),
        		 interpolation * (farm->kd().b()-nearm->kd().b()) + nearm->kd().b());

        return c / 255;
    }else {
        FlatMaterial * farm  = static_cast<FlatMaterial*>(far.get());
        return farm->kd() / 255;
    }
}

void target::DepthMapIntegrator::preprocess( const Scene& scene ){

    for ( int row = 0 ; row < camera.get()->buffer->getHeight(); row++ ) {
        for( int col = 0 ; col < camera.get()->buffer->getWidth() ; col++ ) {
            SurfaceInteraction *isect = new SurfaceInteraction();
            Ray ray = camera.get()->generate_ray( row, col );
            if(scene.intersect(ray, isect)){
                zmin = std::min(zmin, isect->time);
                zmax = std::max(zmax, isect->time);
            }
            delete isect;
        }
    }
}


/*

c = i * (far-near) + near

*/