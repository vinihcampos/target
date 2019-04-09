#ifndef _ORTHO_
#define _ORTHO_

#include "Ray.h"
#include "Camera.h"

namespace target{
	class OrthoCamera : Camera{
		private:
			float l,r,b,t;
		public:
			OrthoCamera(Vec3 look_at, Vec3 look_from, Vec3 vup, float l, float r, float b, float t) 
				: Camera(look_at, look_from, vup), l{l}, r{r}, b{b},t{t}{} 
			Ray generate_ray(int x, int y);	
	};
}

#endif