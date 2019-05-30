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
	
	class TriangleMesh{
		public:
			int nTriangle; // Number of triangles in mesh
			std::vector<int> vertexId;

			std::unique_ptr< Point3[] > points; // The 3D geometric coordinates
    		std::unique_ptr< Vec3[] > normals;  // The 3D normals.
    		std::unique_ptr< Point2[] > uv;     // The 2D texture coordinages (might be 3D, if necessary).

			TriangleMesh( int nTri, const int *indices, int nVertices, const Point3 *p,
              			  const Vec3 *normal, const Point2 *uv ){

			}
		
	};

	class Triangle : public Shape{
		private:
			int * v;
			std::shared_ptr<TriangleMesh> mesh;
			bool backface_cull;

		public:
			Triangle( const std::string & name, std::shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc = true )
            : Shape(name), mesh{mesh}, backface_cull{bfc}{ 
            	v = &mesh->vertexId[ 3 * tri_id ]; 
            }

			Bounds3 object_bound() const;
        	inline bool intersect( Ray& r, SurfaceInteraction * isect) const override{
        		return true;
        	}
			inline bool intersect_p( const Ray& r ) const override{
				return true;
			}
			inline bool intersect_p( const Ray& r, double tmin, double tmax ) const override{
				return true;
			}
	};

}

#endif