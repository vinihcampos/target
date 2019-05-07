#include "NormalMapIntegrator.h"
#include "SurfaceInteraction.h"
#include "Point2.h"
#include "Vec3.h"

target::Color target::NormalIntegrator::Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler ){
    
    SurfaceInteraction *isect = new SurfaceInteraction();
    Ray r = ray;
    if (scene.intersect(r, isect)) {
        Vec3 norm = isect->n.norm();
        Color c( (norm.r() + 1.0f) / 2.0f * 255,
        		 (norm.g() + 1.0f) / 2.0f * 255,
        		 (norm.b() + 1.0f) / 2.0f * 255);
        return c;
    }else {
        return scene.background.get()->sample(camera.get()->buffer, Point2(y,x));
    }
}