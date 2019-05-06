#ifndef _PPM_
#define _PPM_

#include "Buffer.h"
#include <string>

namespace target{

	/**
	* Provides static method to generate PPM image.
	* 
	* @author Vinícius Campos
	* @date 3/12/2019
	*/
	class PPM{
		public:

			/**
			*	Static method to generate PPM image.
			*	@param buffer Buffer to be converted to PPM image.
			*	@param fileName Filename of generated PPM image.
			*	@return std::string The generated PPM image.
			*/
			static std::string generator(std::shared_ptr<Buffer> & buffer, const std::string & fileName = "");
	};
}

#endif