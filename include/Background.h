#ifndef _BACKGROUND_
#define _BACKGROUND_

#include "Buffer.h"
#include "Vec3.h"
#include "Point2.h"
#include <string>

namespace target{

	/**
	* Provides static method to get the color of a point in the buffer.
	* 
	* @author Vinícius Campos
	* @date 3/25/2019
	*/
	class Background{
		public:

			static Color interpolate(const Buffer & buffer, const Point2 p);
	};
}

#endif