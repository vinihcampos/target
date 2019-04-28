#ifndef _SURFACE_INTERACTION_
#define _SURFACE_INTERACTION_

#include "Primitive.h"
#include "Point3.h"
#include "Point2.h"
#include "Vec3.h"

namespace target{

	class Primitive;
	class SurfaceInteraction{
		public:
			SurfaceInteraction(const Point3&p,const Vec3&n, const Vec3&wo, float time, const Point2& uv, const Primitive *pri )
				: p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{pri}{}

			Point3 p; // Contact point.
			
			Vec3 n; // The surface normal.
			
			Vec3 wo; // Outgoing direction of light, which is -ray.
			
			float time; // Time of contact.
			
			Point2 uv; // Parametric coordinate (u,v) of the hit surface.
			
			const Primitive *primitive = nullptr; // Pointer to the primitive.
	};

}

#endif