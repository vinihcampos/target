#ifndef _INTEGRATOR_
#define _INTEGRATOR_

#include "Scene.h"

namespace target{
	class Integrator{
		public:
			virtual void render( const Scene& scene ) = 0;
	};
}

#endif