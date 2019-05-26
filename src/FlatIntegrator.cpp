#include "FlatIntegrator.h"
#include "FlatMaterial.h"
#include "Primitive.h"
#include "Point2.h"
#include "SurfaceInteraction.h"
#include <memory>

target::Color target::FlatIntegrator::Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler, const int & depth ){
    
    SurfaceInteraction *isect = new SurfaceInteraction();
    Ray r = ray;
    if (scene.intersect(r, isect)) {
        FlatMaterial * fm = static_cast<FlatMaterial*>(isect->primitive->get_material());
        return fm->kd() / 255;
    }else {
        return scene.background.get()->sample(camera.get()->buffer, Point2(y,x)) / 255;
    }
}