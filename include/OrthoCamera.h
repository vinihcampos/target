#ifndef _ORTHO_
#define _ORTHO_

#include "Ray.h"
#include "Camera.h"

namespace target{
	class OrthoCamera : Camera{
		public:
			virtual Ray generate_ray(int x, int y);	
	};
}

#endif