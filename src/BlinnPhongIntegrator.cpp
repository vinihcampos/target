#include "BlinnPhongIntegrator.h"
#include "BlinnPhongMaterial.h"
#include "Primitive.h"
#include "Point2.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include <memory>
#include <limits>

void target::BlinnPhongIntegrator::preprocess( const Scene& scene ){
    if(scene.primitives.empty())
        return;

    big_box = scene.primitives[0]->get_bounding_box();

    for(std::shared_ptr<Primitive> pr : scene.primitives){
        big_box = Union(big_box, pr->get_bounding_box());
    }

    worldRadius = big_box.diag().length() / 2;
}


target::Color target::BlinnPhongIntegrator::Li( const Ray& ray, const Scene& scene, Sampler& sampler, const int & depth ){
    
    SurfaceInteraction *isect = new SurfaceInteraction();
    Ray r = ray;
    if (scene.intersect(r, isect)) {
        BlinnPhongMaterial * fm = dynamic_cast<BlinnPhongMaterial*>(isect->primitive->get_material());
        Color kd = fm->kd();
        Color ka = fm->ka();
        Color ks = fm->ks();
        Color km = fm->km();
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
                        Vec3 l = Vec3(Vec3(pl->get_position()) - isect->p);

                        Ray shadow_r = Ray(isect->p, l);
                        if(!scene.intersect_p(shadow_r, 0.001, 1)){
                            l = l.norm();
                            Vec3 h = (v + l) / (v + l).length();
                            color_result += kd * pl->get_intensity() * std::max(0.0, n.dot(l)) +
                                        ks * pl->get_intensity() * std::pow(std::max(0.0, n.dot(h)), glossiness);
                        }
                    }
                    break;
                case LightType::DIRECTIONAL:
                    {
                        DirectionalLight * dl = dynamic_cast<DirectionalLight*>(l.get());
                        Vec3 l = (dl->get_direction()).norm();
                        Point3 pOut = dl->get_direction() * (2 * worldRadius) + isect->p; 
                        Vec3 shadow_l = Vec3(pOut) - Vec3(isect->p);

                        Ray shadow_r = Ray(isect->p, shadow_l);
                        if(!scene.intersect_p(shadow_r, 0.00001, 1)){
                            Vec3 h = (v + l) / (v + l).length();
                            color_result += kd * dl->get_intensity() * std::max(0.0, n.dot(l)) +
                                            ks * dl->get_intensity() * std::pow(std::max(0.0, n.dot(h)), glossiness);
                        }
                    }
                    break;
                case LightType::SPOT:
                    {
                        SpotLight * sl = dynamic_cast<SpotLight*>(l.get());
                        Vec3 l = Vec3(Vec3(sl->get_position()) - isect->p);
                        Vec3 direction = Vec3(sl->get_position()) - Vec3(sl->get_point_at()) ;

                        double cosTheta = l.dot(direction) / (l.length() * direction.length());
                        double delta_intensity;

                        if(cosTheta < sl->get_cutoff()){
                            continue;
                        }else{
                            if(cosTheta > sl->get_falloff())
                                delta_intensity = 1;
                            else{
                                delta_intensity = (cosTheta - sl->get_cutoff()) / (sl->get_falloff() - sl->get_cutoff());
                                delta_intensity = (delta_intensity * delta_intensity) * (delta_intensity * delta_intensity);
                            }

                            Ray shadow_r = Ray(isect->p, l);
                            if(!scene.intersect_p(shadow_r, 0.001, 1)){
                                l = l.norm();
                                Vec3 h = (v + l) / (v + l).length();
                                color_result += kd * sl->get_intensity() * delta_intensity * std::max(0.0, n.dot(l)) +
                                                ks * sl->get_intensity() * delta_intensity * std::pow(std::max(0.0, n.dot(h)), glossiness);
                            }

                        }
                    }
                    break;
                default:
                    break;
            }
        }

        /*if(!(km == Color(0,0,0)) && (depth > 0)){
            Vec3 v_i = ray.getDirection().norm();
            Vec3 reflection = v_i - n * v_i.dot(n.norm()) * 2.0;
            Ray mirror_ray = Ray(isect->p, reflection);
            color_result += km * Li(mirror_ray, scene, sampler, depth - 1);
        }*/

        delete isect;
        return color_result;
    }else {
        double u = 0.0;
        double v = 0.0;
        camera->get_uv(r.getDirection(),u,v);
        Point2 p = camera->generate_point(u,v);
        
        delete isect;
        return scene.background.get()->sample(camera.get()->buffer, Point2(p.x,p.y)) / 255;
    }
}