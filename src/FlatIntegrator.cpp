#include "FlatIntegrator.h"
#include "FlatMaterial.h"
#include "Primitive.h"
#include "Point2.h"
#include <memory>

target::Color target::FlatIntegrator::Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler ){
    
    std::shared_ptr<Primitive> primitive;
    if (scene.intersect_p(ray, primitive)) {
        FlatMaterial * fm = static_cast<FlatMaterial*>(primitive->get_material());
        return fm->kd();
    }else {
        return scene.background.get()->sample(camera.get()->buffer, Point2(y,x));
    }
}