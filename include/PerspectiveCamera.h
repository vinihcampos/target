#ifndef _PERSPECTIVE_
#define _PERSPECTIVE_

#include "Ray.h"
#include "Camera.h"

namespace target{
	class PerspectiveCamera : Camera{
		public:
			virtual Ray generate_ray(int x, int y);	
	};
}

#endif