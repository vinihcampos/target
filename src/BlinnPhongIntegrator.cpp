#include "BlinnPhongIntegrator.h"
#include "BlinnPhongMaterial.h"
#include "Primitive.h"
#include "Point2.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include <memory>

target::Color target::BlinnPhongIntegrator::Li( const Ray& ray, const Scene& scene, int x, int y, Sampler& sampler ){
    
    SurfaceInteraction *isect = new SurfaceInteraction();
    Ray r = ray;
    if (scene.intersect(r, isect)) {
        BlinnPhongMaterial * fm = dynamic_cast<BlinnPhongMaterial*>(isect->primitive->get_material());
        Color kd = fm->kd();
        Color ka = fm->ka();
        Color ks = fm->ks();
        double glossiness = fm->gloss();
        Vec3 n = isect->n.norm();
        Vec3 v = isect->wo.norm();

        Color color_result;

        for (std::shared_ptr<Light> l : scene.lights){
            switch(l->get_type()){
                case LightType::AMBIENT:
                    color_result += ka * l->get_intensity();
                    break;
                case LightType::POINT:
                    {
                        PointLight * pl = dynamic_cast<PointLight*>(l.get());
                        Vec3 l = Vec3(Vec3(isect->p) - pl->get_position()).norm();
                        Vec3 h = (v + l) / (v + l).length();
                        color_result += kd * pl->get_intensity() * std::max(0.0, n.dot(l)) +
                                        ks * pl->get_intensity() * std::pow(std::max(0.0, n.dot(h)), glossiness);
                    }
                    break;
                case LightType::DIRECTIONAL:
                    break;
                case LightType::SPOT:
                    break;
                default:
                    break;
            }
        }
        return (color_result * 255).clamp(0.f, 255.f);
    }else {
        return scene.background.get()->sample(camera.get()->buffer, Point2(y,x));
    }
}