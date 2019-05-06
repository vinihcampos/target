#ifndef _ORTHO_
#define _ORTHO_

#include "Ray.h"
#include "Camera.h"
#include "Buffer.h"

namespace target{
	class OrthoCamera : public Camera{
		public:
			OrthoCamera(std::shared_ptr<Buffer> & buffer, Vec3 look_at, Vec3 look_from, Vec3 vup, int nx, int ny, float l, float r, float b, float t)
			: Camera(buffer, look_at, look_from, vup, nx, ny, l, r, b, t){}
			Ray generate_ray(int x, int y) override;	
	};
}

#endif