#include "NormalMapIntegrator.h"
#include "SurfaceInteraction.h"
#include "Point2.h"
#include "Vec3.h"

target::Color target::NormalMapIntegrator::Li( const Ray& ray, const Scene& scene, Sampler& sampler, const int & depth ){
    
    SurfaceInteraction *isect = new SurfaceInteraction();
    Ray r = ray;
    if (scene.intersect(r, isect)) {
        Vec3 norm = isect->n.norm();
        Color c( std::abs((norm.r() + 1.0f) / 2.0f * 255),
        		 std::abs((norm.g() + 1.0f) / 2.0f * 255),
        		 std::abs((norm.b() + 1.0f) / 2.0f * 255));
        return c / 255;
    }else {
        double u = 0.0;
        double v = 0.0;
        camera->get_uv(r.getDirection(),u,v);
        Point2 p = camera->generate_point(u,v);
        return scene.background.get()->sample(camera.get()->buffer, Point2(p.x,p.y)) / 255;
    }
}