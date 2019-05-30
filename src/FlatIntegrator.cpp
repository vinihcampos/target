#include "FlatIntegrator.h"
#include "FlatMaterial.h"
#include "Primitive.h"
#include "Point2.h"
#include "SurfaceInteraction.h"
#include <memory>

target::Color target::FlatIntegrator::Li( const Ray& ray, const Scene& scene, Sampler& sampler, const int & depth ){
    
    SurfaceInteraction *isect = new SurfaceInteraction();
    Ray r = ray;
    if (scene.intersect(r, isect)) {
        FlatMaterial * fm = static_cast<FlatMaterial*>(isect->primitive->get_material());
        return fm->kd() / 255;
    }else {
        double u = 0.0;
        double v = 0.0;
        camera->get_uv(r.getDirection(),u,v);
        Point2 p = camera->generate_point(u,v);
        return scene.background.get()->sample(camera.get()->buffer, Point2(p.x,p.y)) / 255;
    }
}