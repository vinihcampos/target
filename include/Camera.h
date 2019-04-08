#ifndef _CAMERA_
#define _CAMERA_

#include "Ray.h"

namespace target{
	class Camera{
		public:
			virtual Ray generate_ray(int x, int y) = 0;	
	};
}

#endif