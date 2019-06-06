#ifndef _TRIANGLE_
#define _TRIANGLE_

#include "Primitive.h"
#include "Point3.h"
#include "Point2.h"
#include "Vec3.h"
#include "Bounds3.h"
#include "Ray.h"
#include "SurfaceInteraction.h"
#include <memory>
#include <vector>
#include <string>

namespace target{
	
	const double EPSILON = 0.0000001;

	class TriangleMesh{
		public:
			int nTriangle; // Number of triangles in mesh
			std::vector<int> vertexId;

			std::unique_ptr< Point3[] > points; // The 3D geometric coordinates
    		std::unique_ptr< Vec3[] > normals;  // The 3D normals.
    		std::unique_ptr< Point2[] > uvs;    // The 2D texture coordinages (might be 3D, if necessary).

			TriangleMesh( const std::vector<int> indices, const std::vector<Point3> & ps,
              			  const std::vector<Vec3> & ns, const Point2 *uvs_ = nullptr) {
				nTriangle = indices.size() / 2;
				int nVertices = ps.size();
				vertexId = indices;

				points.reset( new Point3[nVertices] );
				normals.reset( new Vec3[nVertices] );
				//uvs.reset( new Point2[nVertices] );

				for(auto i = 0; i < nVertices; ++i){
					points[i] = ps[i];
					normals[i] = ns[i];
					//uvs[i] = uvs_[i];
				}
			}
		
	};

	class Triangle : public Shape{
		private:
			bool backface_cull;

		public:
			int * id;
			std::shared_ptr<TriangleMesh> mesh;
			Triangle( const std::string & name, std::shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc = false )
            : Shape(name), mesh{mesh}, backface_cull{bfc}{ 
            	id = &mesh->vertexId[ 3 * tri_id ]; 
            }

			Bounds3 object_bound() const;
        	inline bool intersect( Ray& r, SurfaceInteraction * isect) const override{
				Point3 p0 = mesh->points[id[0]]; // Get the 3D coordinate of the 0-vertex of this triangle.
				Point3 p1 = mesh->points[id[1]]; // Same for the 1-vertex.
				Point3 p2 = mesh->points[id[2]]; // Same for the 2-vertex.

				Vec3 edge1, edge2, t, p, q;
				double det, inv_det, u, v;

				edge1 = Vec3(p1) - p0;
				edge2 = Vec3(p2) - p0;

				p = r.getDirection().cross(edge2);
				det = edge1.dot(p);

				if(backface_cull){
					if(det < EPSILON) return false;

					t = Vec3(r.getOrigin()) - p0;
					u = t.dot(p);
					if(u < 0.0 || u > det) return false;

					q = t.cross(edge1);
					v = r.getDirection().dot(q);

					if(v < 0.0 || u + v > det) return false;

					double hit_t = edge2.dot(q);
					inv_det = 1.0 / det;

					hit_t *= inv_det;
					u *= inv_det;
					v *= inv_det;

					if(hit_t <= EPSILON) return false;
					if(isect != nullptr){
			    		isect->time = hit_t;
			    		isect->p = r(hit_t);
			    		isect->wo = (Vec3(r.getOrigin()) - r.getDirection());

			    		Vec3 n1 = mesh->normals[id[0]];
			    		Vec3 n2 = mesh->normals[id[1]];
			    		Vec3 n3 = mesh->normals[id[2]];

			    		isect->n =  (n1*(1-u-v) + n2*u + n3*v).norm();
			    	}
				}else{
					if(det > -EPSILON && det < EPSILON) return 0;
					inv_det = 1.0 / det;

					t = Vec3(r.getOrigin()) - p0;

					u = t.dot(p) * inv_det;
					if(u < 0.0 || u > 1.0) return false;

					q = t.cross(edge1);
					v = r.getDirection().dot(q) * inv_det;
					if(v < 0.0 || u + v > 1.0) return false;

					double hit_t = edge2.dot(q) * inv_det;
					if(hit_t <= EPSILON) return false;
					if(isect != nullptr){
			    		isect->time = hit_t;
			    		isect->p = r(hit_t);
			    		isect->wo = (Vec3(r.getOrigin()) - r.getDirection());

			    		Vec3 n1 = mesh->normals[id[0]];
			    		Vec3 n2 = mesh->normals[id[1]];
			    		Vec3 n3 = mesh->normals[id[2]];

			    		isect->n =  (n1*(1-u-v) + n2*u + n3*v).norm();
			    	}				
 				}
 				return true;
        	}
			inline bool intersect_p( const Ray& r ) const override{
				Ray ray = r;
				return intersect(ray, nullptr);
			}
			inline bool intersect_p( const Ray& r, double tmin, double tmax ) const override{
				SurfaceInteraction * s = new SurfaceInteraction();
				Ray ray = r;
				return intersect(ray, s) && s->time >= tmin && s->time <= tmax;
			}
			inline const Material * get_material(void) const { return this->material.get(); }
	};

}

#endif