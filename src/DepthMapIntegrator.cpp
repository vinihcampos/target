#include "DepthMapIntegrator.h"
#include "SurfaceInteraction.h"
#include "FlatMaterial.h"
#include "Point2.h"
#include "Vec3.h"
#include <cmath>

target::Color target::DepthMapIntegrator::Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler ){

    SurfaceInteraction *isect = new SurfaceInteraction();
    Ray r = ray;
    if (scene.intersect(r, isect)) {
        float t = isect->time;
        float interpolation = (t - zmin) / (zmax - zmin);

        FlatMaterial * farm  = static_cast<FlatMaterial*>(far.get());
        FlatMaterial * nearm = static_cast<FlatMaterial*>(near.get());

        Color c( interpolation * (farm->kd().r()-nearm->kd().r()) + nearm->kd().r(),
        		 interpolation * (farm->kd().g()-nearm->kd().g()) + nearm->kd().g(),
        		 interpolation * (farm->kd().b()-nearm->kd().b()) + nearm->kd().b());
        return c;
    }else {
        return scene.background.get()->sample(camera.get()->buffer, Point2(y,x));
    }
}

void target::DepthMapIntegrator::preprocess( const Scene& scene ){

    SurfaceInteraction *isect = new SurfaceInteraction();
    for ( int row = 0 ; row < camera.get()->buffer->getHeight(); row++ ) {
        for( int col = 0 ; col < camera.get()->buffer->getWidth() ; col++ ) {
            Ray ray = camera.get()->generate_ray( row, col );
            if(scene.intersect(ray, isect)){
                zmin = std::min(zmin, isect->time);
                zmax = std::max(zmax, isect->time);
            }
        }
    }

    delete isect;
}


/*

c = i * (far-near) + near

*/